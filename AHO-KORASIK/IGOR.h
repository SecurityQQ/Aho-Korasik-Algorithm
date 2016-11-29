//
//  AK-Automata.h
//  AHO-KORASIK
//
//  Created by Alexander Malyshev on 13.11.15.
//  Copyright © 2015 Alexander Malyshev. All rights reserved.
//

#ifndef AK_Automata_h
#define AK_Automata_h

#ifndef IAHO_AUTOMATA_CONFIG_H
#define IAHO_AUTOMATA_CONFIG_H

#define NO_NODE -1

class IAhoAutomataConfig
{
public:
    virtual unsigned long long GetLetterId(char c) = 0;
    virtual unsigned long long GetAlphabetSize() = 0;
    virtual bool AssertCorrectSymbol(char c) = 0;
};

#endif
#ifndef ENGLISH_AHO_AUTOMATA_CONFIG
#define ENGLISH_AHO_AUTOMATA_CONFIG


#include <unordered_map>

class EnglishAhoAutomataConfig: public IAhoAutomataConfig
{
public:
    EnglishAhoAutomataConfig();
    unsigned long long GetLetterId(char c);
    unsigned long long GetAlphabetSize();
    bool AssertCorrectSymbol(char c);
private:
    unsigned long long alphabetSize_;
    std::unordered_map<char, unsigned long long> lettersCodes_;
};

#endif
#ifndef CONFIG_FACTORY_H
#define CONFIG_FACTORY_H


class AhoConfigFactory{
public:
    static IAhoAutomataConfig& Config();
};

#endif
#ifndef AHO_AUTOMATA_H
#define AHO_AUTOMATA_H

#include <string>
#include <vector>
#include <istream>

#define ROOT_ID 0

class AutomataNode;

class AhoAutomata
{
public:
    AhoAutomata(IAhoAutomataConfig& config, std::vector<std::string>& strings);
    AhoAutomata(IAhoAutomataConfig& config);
    ~AhoAutomata();
    void FindTemplates(std::istream& input, std::ostream& output);
protected:
    void AddString(std::string s);
    std::vector<AutomataNode*> nodes_;
    AutomataNode* GetTransition(AutomataNode* node, char c);
    AutomataNode* GetFastSuffixLink(AutomataNode* node);
    std::vector<std::string> strings_;
    IAhoAutomataConfig& config_;
private:
    void AddString(std::string s, unsigned long long stringId);
    AutomataNode* GetNode(long long id);
    AutomataNode* GetSuffixLink(AutomataNode* node);
    AutomataNode* GetSuffixLink(long long nodeId);
    AutomataNode* GetTransition(long long nodeId, char c);
    unsigned long long size_;
};

#endif
#ifndef AUTOMATA_NODE_H
#define AUTOMATA_NODE_H

#include <vector>
#include <memory>

class AhoAutomata;

class AutomataNode
{
public:
    AutomataNode(AhoAutomata& automata, long long id, long long parentId, IAhoAutomataConfig& config, char symbol = '\0',
                 long long stringId = NO_NODE);
    long long GetChildId(char c);
    long long GetId();
    char GetSymbol();
    bool IsString();
    std::vector<long long>& GetStrings();
    long long GetTransition(char c);
    long long GetSuffixLinkId();
    long long GetFastLinkId();
    long long GetParentId();
    void SetSuffixLinkId(long long id);
    void SetFastLinkId(long long id);
    void SetTransition(char c, long long transition);
    void AddChild(char c, long long childId);
    void MarkAsString(long long stringId);
private:
    long long id_;
    char symbol_;
    long long parentId_;
    long long suffix_;
    std::vector<long long> childs_;
    std::vector<long long> transitions_;
    std::vector<long long> string_;
    long long fastSuffix_;
    IAhoAutomataConfig& config_;
    AhoAutomata& automata_;
};

#endif
#ifndef AHOMASKAUTOMATA_H
#define AHOMASKAUTOMATA_H


class AhoMaskAutomata: public AhoAutomata
{
public:
    AhoMaskAutomata(IAhoAutomataConfig& config, std::istream& input);
    void FindTemplate(std::istream& input, std::ostream& output);
private:
    std::vector<long long> subPatternStartPosition_;
    long long patternSize_;
};

#endif

#include <assert.h>

AhoAutomata::AhoAutomata(IAhoAutomataConfig& config, std::vector<std::string>& strings): config_(config), size_(1), strings_(strings)
{
    this->nodes_.push_back(new AutomataNode(*this, 0, NO_NODE, config_));
    for(unsigned long long i = 0; i < strings.size(); ++i)
        this->AddString(strings[i], i);
}
AhoAutomata::AhoAutomata(IAhoAutomataConfig& config):config_(config), size_(1)
{
    this->nodes_.push_back(new AutomataNode(*this, 0, NO_NODE, config_));
}

AhoAutomata::~AhoAutomata()
{
    for(auto n = nodes_.begin(); n != nodes_.end(); ++n)
        delete (*n);
}

void AhoAutomata::FindTemplates(std::istream& input, std::ostream& output)
{
    std::ios::sync_with_stdio(false);
    char c;
    long long position = 0;
    auto state = this->nodes_.at(ROOT_ID);
    while(input.get(c) && c != '\n')
    {
        ++position;
        state = this->GetTransition(state, c);
        for(auto st = state; st->GetId() != ROOT_ID; st = GetFastSuffixLink(st))
        {
            if(st->IsString())
                for(auto s = st->GetStrings().begin(); s != st->GetStrings().end(); ++s)
                    output << position - strings_.at(*s).length() << strings_.at(*s) << std::endl;
        }
    }
    std::ios::sync_with_stdio(true);
}
void AhoAutomata::AddString(std::string s)
{
    strings_.push_back(s);
    this->AddString(s, strings_.size() - 1);
}
void AhoAutomata::AddString(std::string s, unsigned long long stringId)
{
    long long current = 0;
    assert(s.size() != 0);
    for(long long i = 0 ; i < s.size(); ++i)
    {
        auto next = nodes_.at(current)->GetChildId(s[i]);
        if(next == NO_NODE) {
            nodes_.push_back(new AutomataNode(*this, size_, current, config_, s[i]));
            nodes_.at(current)->AddChild(s[i], size_);
            current = size_;
            ++size_;
        }
        else
            current = next;
    }
    this->nodes_.at(current)->MarkAsString(stringId);
}
AutomataNode* AhoAutomata::GetNode(long long id)
{
    assert(id >= 0 && id < nodes_.size());
    return nodes_.at(id);
}
AutomataNode*AhoAutomata::GetSuffixLink(AutomataNode* node)
{
    if(node->GetSuffixLinkId() == NO_NODE)
        if(node->GetId() == ROOT_ID || node->GetParentId() == ROOT_ID)
            node->SetSuffixLinkId(ROOT_ID);
        else
            node->SetSuffixLinkId(this->GetTransition(GetSuffixLink(node->GetParentId()), node->GetSymbol())->GetId());
    assert(node->GetSuffixLinkId() != NO_NODE);
    return nodes_.at(node->GetSuffixLinkId());
}
AutomataNode* AhoAutomata::GetFastSuffixLink(AutomataNode* node)
{
    if(node->GetFastLinkId() == NO_NODE) {
        auto link = GetSuffixLink(node);
        if(link->GetId() == ROOT_ID)
            node->SetFastLinkId(ROOT_ID);
        else
            node->SetFastLinkId(link->IsString() ? link->GetId() : GetSuffixLink(link)->GetId());
    }
    assert(node->GetSuffixLinkId() != NO_NODE);
    return nodes_.at(node->GetFastLinkId());
}
AutomataNode* AhoAutomata::GetTransition(AutomataNode* node, char c)
{
    if(node->GetTransition(c) == NO_NODE)
        if(node->GetChildId(c) != NO_NODE)
            node->SetTransition(c, node->GetChildId(c));
        else
            if(node->GetId() == ROOT_ID)
                node->SetTransition(c, ROOT_ID);
            else
                node->SetTransition(c, this->GetTransition(this->GetSuffixLink(node), c)->GetId());
    assert(node->GetTransition(c) != NO_NODE);
    return this->nodes_.at(node->GetTransition(c));
}
AutomataNode* AhoAutomata::GetSuffixLink(long long nodeId)
{
    assert(nodeId < nodes_.size());
    return this->GetSuffixLink(nodes_.at(nodeId));
}
AutomataNode*AhoAutomata::GetTransition(long long nodeId, char c)
{
    assert(nodeId < nodes_.size());
    return this->GetTransition(nodes_.at(nodeId), c);
}

#include <assert.h>

AutomataNode::AutomataNode(AhoAutomata& automata, long long id, long long parentId, IAhoAutomataConfig& config, char symbol,
                           long long stringId):id_(id), symbol_(symbol), parentId_(parentId), suffix_(NO_NODE),
fastSuffix_(NO_NODE), config_(config), automata_(automata)
{
    this->childs_.assign(config.GetAlphabetSize(), NO_NODE);
    this->transitions_.assign(config.GetAlphabetSize(), NO_NODE);
    if(stringId != NO_NODE)
        string_.push_back(stringId);
}

long long AutomataNode::GetChildId(char c)
{
    return childs_.at(config_.GetLetterId(c));
}
long long AutomataNode::GetId()
{
    return this->id_;
    
}
char AutomataNode::GetSymbol()
{
    return symbol_;
}
bool AutomataNode::IsString()
{
    return string_.size() > 0;
}
std::vector<long long>& AutomataNode::GetStrings()
{
    return string_;
}
long long AutomataNode::GetTransition(char c)
{
    auto letterId = config_.GetLetterId(c);
    return this->transitions_.at(letterId);
}

long long AutomataNode::GetSuffixLinkId()
{
    return this->suffix_;
}
long long AutomataNode::GetFastLinkId()
{
    return this->fastSuffix_;
}
long long AutomataNode::GetParentId()
{
    return this->parentId_;
}
void AutomataNode::SetSuffixLinkId(long long id)
{
    this->suffix_ = id;
}
void AutomataNode::SetFastLinkId(long long id)
{
    this->fastSuffix_ = id;
}
void AutomataNode::SetTransition(char c, long long transition)
{
    this->transitions_[config_.GetLetterId(c)] = transition;
}
void AutomataNode::AddChild(char c, long long childId)
{
    assert(this->childs_.at(config_.GetLetterId(c)) == NO_NODE);
    this->childs_[config_.GetLetterId(c)] = childId;
}
void AutomataNode::MarkAsString(long long stringId)
{
    this->string_.push_back(stringId);
}

EnglishAhoAutomataConfig::EnglishAhoAutomataConfig()
{
    this->alphabetSize_ = 26;
    this->lettersCodes_.insert({{'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}, {'e', 4}, {'f', 5}, {'g', 6}, {'h', 7}, {'i', 8}, {'j', 9},
        {'k', 10}, {'l', 11}, {'m', 12}, {'n', 13}, {'o', 14}, {'p', 15}, {'q', 16}, {'r', 17}, {'s', 18},
        {'t', 19}, {'u', 20}, {'v', 21}, {'w', 22}, {'x', 23}, {'y', 24}, {'z', 25}});
}

unsigned long long EnglishAhoAutomataConfig::GetLetterId(char c)
{
    /*if(c < 'a' || c > 'z')
     return 0;*/
    return lettersCodes_.at(c);
}
unsigned long long EnglishAhoAutomataConfig::GetAlphabetSize()
{
    return alphabetSize_;
}
bool EnglishAhoAutomataConfig::AssertCorrectSymbol(char c)
{
    return (c >= 'a' && c <= 'z');
}

#include <assert.h>

AhoMaskAutomata::AhoMaskAutomata(IAhoAutomataConfig& config, std::istream& input):AhoAutomata(config)
{
    std::ios::sync_with_stdio(false);
    std::string s = "";
    long long subPatternStartPosition = 0;
    char c;
    while(input.get(c) && c != '\n')
    {
        if(!(config_.AssertCorrectSymbol(c) || c == '?'))
            continue;
        if(c == '?') {
            if(s.compare("") != 0) {
                this->AddString(s);
                s = "";
            }
        }
        else
        {
            if(s.compare("") == 0)
                subPatternStartPosition_.push_back(subPatternStartPosition);
            s += c;
        }
        ++subPatternStartPosition;
    }
    if(s.compare("") != 0)
        this->AddString(s);
    patternSize_ = subPatternStartPosition;
    std::ios::sync_with_stdio(true);
}
void AhoMaskAutomata::FindTemplate(std::istream& input, std::ostream& output)
{
    std::ios::sync_with_stdio(false);
    char c;
    long long position = 0;
    auto state = this->nodes_.at(ROOT_ID);
    std::vector<long long> subPatternCounter;
    long long patternCounter = 0;
    while(input.get(c))
    {
        if(!config_.AssertCorrectSymbol(c))
            continue;
        subPatternCounter.push_back(0);
        ++position;
        state = this->GetTransition(state, c);
        for(auto st = state; st->GetId() != ROOT_ID; st = GetFastSuffixLink(st))
        {
            if(st->IsString())
                for(auto s = st->GetStrings().begin(); s != st->GetStrings().end(); ++s)
                {
                    auto index = position - (long long)strings_.at(*s).length() - (long long)subPatternStartPosition_.at(*s);
                    if(index >= 0)
                        ++subPatternCounter[index];
                }
        }
    }
    
    for(long long i = 0; patternSize_ - 1 +  i < subPatternCounter.size(); ++i)
        if(subPatternCounter[i] >= subPatternStartPosition_.size()) {
            output << i << " ";
            ++patternCounter;
        }
    
    std::ios::sync_with_stdio(true);
}

IAhoAutomataConfig& AhoConfigFactory::Config()
{
    static EnglishAhoAutomataConfig conf;
    return conf;
}
#include <iostream>

//
//int main()
//{
//    auto a = AhoMaskAutomata(AhoConfigFactory::Config(), std::cin);
//    a.FindTemplate(std::cin, std::cout);
//    return 0;
//}


#endif /* AK_Automata_h */
