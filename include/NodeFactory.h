/**
 * @file NodeFactory.h
 * @brief Factory for construction/destruction/accounting of nodes having generic item Data.
 *        Interface and template file.
 * @author Guillermo M. Paris
 * @date 2019-10-27
 */

#ifndef _NODEFACTORY_H
#define _NODEFACTORY_H

#include <cassert>
#include <vector>
#include "Node.h"

template<class Data>
class NodeFactory
{
public:
    static NodeFactory* getOrCreateInstance();
    static void         destroyInstance();

    Node<Data>* createNode(const Data& d);
    Node<Data>* createNode(const Data& d, const Node<Data>* pp);
    Node<Data>* createNode(const Data& d, const Node<Data>* pl, const Node<Data>* pp, const Node<Data>* pr);
    Node<Data>* createNode(Data&& d);
    Node<Data>* createNode(Data&& d, const Node<Data>* pp);
    Node<Data>* createNode(Data&& d, const Node<Data>* pl, const Node<Data>* pp, const Node<Data>* pr);

    void        destroyNode(Node<Data>* pNode);

private:
    NodeFactory()  {nSequence = 0; vAllocatedNodes.reserve(50);}
    ~NodeFactory() {destroyAll();}

    void destroyAll();
    void checkMemoryAssignement(Node<Data>* const pnew);

    static NodeFactory* pInstance;

    unsigned int               nSequence;
    std::vector<Node<Data>*>   vAllocatedNodes;
};

template<class Data>
NodeFactory<Data>* NodeFactory<Data>::getOrCreateInstance()
{
    if (pInstance == nullptr)
        return pInstance = new NodeFactory<Data>;
    else
        return pInstance;
}

template<class Data>
void NodeFactory<Data>::destroyInstance()
{
    if (pInstance == nullptr)
        return;

    delete pInstance;
    pInstance = nullptr;
}

template<class Data>
Node<Data>* NodeFactory<Data>::createNode(const Data& d)
{
    Node<Data>* pNew = new Node<Data>(d, nSequence);
    checkMemoryAssignement(pNew);
    return pNew;
}

template<class Data>
Node<Data>* NodeFactory<Data>::createNode(const Data& d, const Node<Data>* pp)
{
    Node<Data>* pNew = new Node<Data>(d, nSequence, pp);
    checkMemoryAssignement(pNew);
    return pNew;
}

template<class Data>
Node<Data>* NodeFactory<Data>::createNode(const Data& d, const Node<Data>* pl, const Node<Data>* pp, const Node<Data>* pr)
{
    Node<Data>* pNew = new Node<Data>(d, nSequence, pp, pr);
    checkMemoryAssignement(pNew);
    return pNew;
}

template<class Data>
Node<Data>* NodeFactory<Data>::createNode(Data&& d)
{
    Node<Data>* pNew = new Node<Data>(d, nSequence);
    checkMemoryAssignement(pNew);
    return pNew;
}

template<class Data>
Node<Data>* NodeFactory<Data>::createNode(Data&& d, const Node<Data>* pp)
{
    Node<Data>* pNew = new Node<Data>(d, nSequence, pp);
    checkMemoryAssignement(pNew);
    return pNew;
}

template<class Data>
Node<Data>* NodeFactory<Data>::createNode(Data&& d, const Node<Data>* pl, const Node<Data>* pp, const Node<Data>* pr)
{
    Node<Data>* pNew = new Node<Data>(d, nSequence, pp, pr);
    checkMemoryAssignement(pNew);
    return pNew;
}

template<class Data>
void NodeFactory<Data>::checkMemoryAssignement(Node<Data>* const pnew)
{
    if (pnew == nullptr)
    {
        destroyAll();
        assert(true);
    }
    else
    {
        pnew->nSequence = nSequence++;
        vAllocatedNodes.push_back(pnew);
    }
}

template<class Data>
void NodeFactory<Data>::destroyNode(Node<Data>* pNode)
{
    if (pNode == nullptr) return;
    
    vAllocatedNodes[pNode->nSequence] = nullptr;
    delete pNode;
}

template<class Data>
void NodeFactory<Data>::destroyAll()
{
    for (Node<Data>* & pNode :  vAllocatedNodes)
        if (pNode != nullptr)
        {
            delete pNode;
            pNode = nullptr;
        }

    nSequence = 0;
    vAllocatedNodes.clear();
}

#endif // _NODEFACTORY_H
