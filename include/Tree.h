/**
 * @file Node.h
 * @brief Binary Tree made of nodes of generic item Data (Data templetized and composed into).
 *        Interface and template file.
 * @author Guillermo M. Paris
 * @date 2019-10-27
 */

#ifndef _TREE_H
#define _TREE_H

#include <cassert>
#include <functional>
#include "Node.h"

template<class Data>
class Tree
{
//  using searchPredicate = bool (*)(const Node<Data>*); // former way - function pointer
    using searchPredicate = std::function<bool(const Node<Data>*)>; // modern way - lambdas

public:
    Tree(Node<Data>* root)     {pCurrentNode = pRootNode = root; root->setParent(nullptr);}
    ~Tree()                    {} // this Tree implementation does not allocate anything.

    Node<Data>* getRoot()  const  {return pRootNode;}
    Node<Data>* getCurrent()  const  {return pCurrentNode;}
    bool        isRoot(const Node<Data>* pNode)  const  {return pNode == pRootNode;}
    void        setRootAndCurrent(Node<Data>* const root) {pCurrentNode = pRootNode = root;}

    Node<Data>* skipDownLeft()  {Node<Data>* pRet = pCurrentNode->getLeft();  return pRet ? pCurrentNode = pRet : nullptr;}
    Node<Data>* skipUp()        {Node<Data>* pRet = pCurrentNode->geParent(); return pRet ? pCurrentNode = pRet : nullptr;}
    Node<Data>* skipDownRight() {Node<Data>* pRet = pCurrentNode->getRight(); return pRet ? pCurrentNode = pRet : nullptr;}

    Node<Data>* searchDownLeft(searchPredicate, bool notFoundNoMove = false);
    Node<Data>* searchUp(searchPredicate, bool notFoundNoMove = false);
    Node<Data>* searchDownRight(searchPredicate, bool notFoundNoMove = false);

    Node<Data>* goTop()         {return pCurrentNode = pRootNode;}
    Node<Data>* goDownLeftMost();
    Node<Data>* goDownRightMost();
    Node<Data>* goLeftDownMost();
    Node<Data>* goRightDownMost();

    void makeLeftHalfTree(Node<Data>* const newRoot);
    void makeRightHalfTree(Node<Data>* const newRoot);
    bool deleteCurrentPreserveLeft(bool currentGoUp = false);
    bool deleteCurrentPreserveRight(bool currentGoUp = false);
    bool insertOverCurrentOnLeft(Node<Data>* newNode, bool subtreeTruncationCheck = false);
    bool insertOverCurrentOnRight(Node<Data>* newNode, bool subtreeTruncationCheck = false);
    bool insertUnderCurrentOnLeft(Node<Data>* newNode, bool zigzag = false, bool subtreeTruncationCheck = false);
    bool insertUnderCurrentOnRight(Node<Data>* newNode, bool zigzag = false, bool subtreeTruncationCheck = false);

private:
    bool hangPreferablyLeftOfCurrentParentNode(Node<Data>* newNode);
    bool hangPreferablyRightOfCurrentParentNode(Node<Data>* newNode);

    Node<Data>*  pRootNode;
    Node<Data>*  pCurrentNode;
};

template<class Data>
Node<Data>* Tree<Data>::searchUp(searchPredicate fp, bool notFoundNoMove /* = false */)
{
    Node<Data>* pLower = pCurrentNode;
    Node<Data>* pNode = pCurrentNode;
    while (pNode != nullptr && fp(pNode))
    {
        pLower = pNode;
        pNode = pNode->getParent();
    }

    if(pNode == nullptr && notFoundNoMove) // if data not found and move the current is disabled when not data found.
        return nullptr;
    else // if data found, or move the current is enabled even not data found.
        return pCurrentNode = (pNode == nullptr ? pLower : pNode);
}

template<class Data>
Node<Data>* Tree<Data>::searchDownLeft(searchPredicate fp, bool notFoundNoMove /* = false */)
{
    Node<Data>* pUpper = pCurrentNode;
    Node<Data>* pNode = pCurrentNode;
    while (pNode != nullptr && fp(pNode))
    {
        pUpper = pNode;
        pNode = pNode->getLeft();
    }

    if(pNode == nullptr && notFoundNoMove) // if data not found and move the current is disabled when not data found.
        return nullptr;
    else // if data found, or move the current is enabled even not data found.
        return pCurrentNode = (pNode == nullptr ? pUpper : pNode);
}

template<class Data>
Node<Data>* Tree<Data>::searchDownRight(searchPredicate fp, bool notFoundNoMove /* = false */)
{
    Node<Data>* pUpper = pCurrentNode;
    Node<Data>* pNode = pCurrentNode;
    while (pNode != nullptr && fp(pNode))
    {
        pUpper = pNode;
        pNode = pNode->getRight();
    }

    if(pNode == nullptr && notFoundNoMove) // if data not found and move the current is disabled when not data found.
        return nullptr;
    else // if data found, or move the current is enabled even not data found.
        return pCurrentNode = (pNode == nullptr ? pUpper : pNode);
}

template<class Data>
Node<Data>* Tree<Data>::goDownLeftMost()
{
    Node<Data>* pUpper = nullptr;
    Node<Data>* pNode = pCurrentNode;
    do
    {
        pUpper = pNode;
        pNode = pNode->getLeft();
    } while (pNode != nullptr);

    return pCurrentNode = pUpper;
}

template<class Data>
Node<Data>* Tree<Data>::goDownRightMost()
{
    Node<Data>* pUpper = nullptr;
    Node<Data>* pNode = pCurrentNode;
    do
    {
        pUpper = pNode;
        pNode = pNode->getRight();
    } while (pNode != nullptr);

    return pCurrentNode = pUpper;
}

template<class Data>
Node<Data>* Tree<Data>::goLeftDownMost()
{
    Node<Data>* pUpper = nullptr;
    Node<Data>* pNode = pCurrentNode;
    do
    {
        pUpper = pNode;
        pNode = pNode->getLeft();
        if (pNode == nullptr)
          pNode = pUpper->getRight();
    } while (pNode != nullptr);

    return pCurrentNode = pUpper;
}

template<class Data>
Node<Data>* Tree<Data>::goRightDownMost()
{
    Node<Data>* pUpper = nullptr;
    Node<Data>* pNode = pCurrentNode;
    do
    {
        pUpper = pNode;
        pNode = pNode->getRight();
        if (pNode == nullptr)
          pNode = pUpper->getLeft();
    } while (pNode != nullptr);

    return pCurrentNode = pUpper;
}

template<class Data>
void Tree<Data>::makeLeftHalfTree(Node<Data>* const newRoot)
{
    pRootNode->setParent(newRoot);
    newRoot->setLeft(pRootNode);
    newRoot->setParent(nullptr);
    pCurrentNode = pRootNode = newRoot;
}

template<class Data>
void Tree<Data>::makeRightHalfTree(Node<Data>* const newRoot)
{
    pRootNode->setParent(newRoot);
    newRoot->setRight(pRootNode);
    newRoot->setParent(nullptr);
    pCurrentNode = pRootNode = newRoot;
}

template<class Data>
bool Tree<Data>::deleteCurrentPreserveLeft(bool currentGoUp /* = false */)
{
    if (pCurrentNode == nullptr)
        return false; // void Tree protection

    Node<Data>* parent = pCurrentNode->getParent();
    if (parent == nullptr)
        return false; // root node can be changed BUT NEVER deleted

    Node<Data>* left = pCurrentNode->getLeft();
    hangPreferablyLeftOfCurrentParentNode(left);
    if (left == nullptr || currentGoUp) // there is not a node on the left, under the current.
    {
        pCurrentNode = parent; // current position is on the parent;
    }
    else // there is a node under left part of the current node.
    {
        pCurrentNode = left; // current position is on the "preserved left"
    }

    return true;
}

template<class Data>
bool Tree<Data>::deleteCurrentPreserveRight(bool currentGoUp /* = false */)
{
    if (pCurrentNode == nullptr)
        return false; // void Tree protection

    Node<Data>* parent = pCurrentNode->getParent();
    if (parent == nullptr)
        return false; // root node can be changed BUT NEVER deleted

    Node<Data>* right = pCurrentNode->getRight();
    hangPreferablyRightOfCurrentParentNode(right);
    if (right == nullptr || currentGoUp) // there is not a node on the right, under the current.
    {
        pCurrentNode = parent; // new current position is on the parent, now.
    }
    else // there is a node under right part of the current node.
    {
        pCurrentNode = right; // new current position is on the "preserved right", now.
    }

    return true;
}

template<class Data>
bool Tree<Data>::insertOverCurrentOnLeft(Node<Data>* newNode, bool subtreeTruncationCheck /* = false */)
{
    if (subtreeTruncationCheck && newNode->getLeft() != nullptr)
        return false; // left subtree below newNode was going to be lost.

    Node<Data>* parent = pCurrentNode->getParent();
    if (parent == nullptr)
    {
        makeLeftHalfTree(newNode); // current node is the root.
        return true;
    }

    newNode->setLeft(pCurrentNode);
    hangPreferablyLeftOfCurrentParentNode(newNode);
    pCurrentNode->setParent(newNode);
    return true;
}

template<class Data>
bool Tree<Data>::insertOverCurrentOnRight(Node<Data>* newNode, bool subtreeTruncationCheck /* = false */)
{
   if (subtreeTruncationCheck && newNode->getRight() != nullptr)
        return false; // right subtree below newNode was going to be lost.

    Node<Data>* parent = pCurrentNode->getParent();
    if (parent == nullptr)
    {
        makeRightHalfTree(newNode); // current node is the root.
        return true;
    }

    newNode->setRight(pCurrentNode);
    hangPreferablyRightOfCurrentParentNode(newNode);
    pCurrentNode->setParent(newNode);
    return true;
}

template<class Data>
bool Tree<Data>::hangPreferablyLeftOfCurrentParentNode(Node<Data>* newNode)
{
    Node<Data>* parent = pCurrentNode->getParent();

    if (parent->getLeft() == pCurrentNode) // current is on the left of parent.
        parent->setLeft(newNode);
    else if (parent->getRight() == pCurrentNode) // current is on the right of parent.
        parent->setRight(newNode);
    else // INCONSISTENCE parent node does not know current is its child.
    {
        assert(false);
        return false; // just in case
    }

    if (newNode != nullptr) // null argument may appear if this is called from deleteCurrentPreserveLeft().
        newNode->setParent(parent);

    return true;
}

template<class Data>
bool Tree<Data>::hangPreferablyRightOfCurrentParentNode(Node<Data>* newNode)
{
    Node<Data>* parent = pCurrentNode->getParent();

    if (parent->getRight() == pCurrentNode) // current is on the right of parent.
        parent->setRight(newNode);
    else if (parent->getLeft() == pCurrentNode) // current is on the left of parent.
        parent->setLeft(newNode);
    else // INCONSISTENCE parent node does not know current is its child.
    {
        assert(false);
        return false; // just in case
    }

    if (newNode != nullptr) // null argument may appear if this is called from deleteCurrentPreserveRight().
        newNode->setParent(parent);

    return true;
}

template<class Data>
bool Tree<Data>::insertUnderCurrentOnLeft( Node<Data>* newNode, bool zigzag /* = false */,
                                                                bool subtreeTruncationCheck /* = false */ )
{
    if (subtreeTruncationCheck)
    {
        if (!zigzag && newNode->getLeft() != nullptr)
            return false; // left subtree below newNode was going to be lost.
        else if (zigzag && newNode->getRight() != nullptr)
            return false; // right subtree below newNode was going to be lost.
    }

    Node<Data>* underCurrentNode = pCurrentNode->getLeft();
    pCurrentNode->setLeft(newNode);
    newNode->setParent(pCurrentNode);
    if (zigzag)
        newNode->setRight(underCurrentNode);
    else
        newNode->setLeft(underCurrentNode);

    if (underCurrentNode != nullptr)
        underCurrentNode->setParent(newNode);

    return true;
}

template<class Data>
bool Tree<Data>::insertUnderCurrentOnRight( Node<Data>* newNode, bool zigzag /* = false */,
                                                                 bool subtreeTruncationCheck /* = false */ )
{
    if (subtreeTruncationCheck)
    {
        if (!zigzag && newNode->getLeft() != nullptr)
            return false; // left subtree below newNode was going to be lost.
        else if (zigzag && newNode->getRight() != nullptr)
            return false; // right subtree below newNode was going to be lost.
    }

    Node<Data>* underCurrentNode = pCurrentNode->getRight();
    pCurrentNode->setRight(newNode);
    newNode->setParent(pCurrentNode);
    if (zigzag)
        newNode->setLeft(underCurrentNode);
    else
        newNode->setRight(underCurrentNode);

    if (underCurrentNode != nullptr)
        underCurrentNode->setParent(newNode);

    return true;
}


#endif // _TREE_H
