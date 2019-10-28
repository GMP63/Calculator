/**
 * @file Node.h
 * @brief Node of generic item Data (Data templetized and composed into). Interface and template file.
 * @author Guillermo M. Paris
 * @date 2019-10-27
 */

#ifndef _NODE_H
#define _NODE_H

template <class Data> class NodeFactory;

template <class Data>
class Node
{
    friend class NodeFactory<Data>;

public:
    const Data& getData() const     {return data;}
    int   getSequenceNo() const     {return nSequence;}
    Node* getLeft()       const     {return pLeft;}
    Node* getParent()     const     {return pParent;}
    Node* getRight()      const     {return pRight;}

    void  setData(const Data& d)    {data = d;}
    void  setData(Data&& d)         {data = d;}
    void  setLeft(Node* const pl)   {pLeft = pl;}
    void  setParent(Node* const pp) {pParent = pp;}
    void  setRight(Node* const pr)  {pRight = pr;}

    bool  isLeftChildOf(Node* const pn)  const {return pn->pLeft == this && pn == pParent;}
    bool  isParentOf(Node* const pn)     const {return (pLeft == pn || pRight == pn)
                                                        && pn->pParent == this;}
    bool  isRightChildOf(Node* const pn) const {return pn->pRight == this && pn == pParent;}
    bool  isChildOf(Node* const pn)      const {return (pn->pLeft == this || pn->pRight == this)
                                                        && pn == pParent;}

private:
    Node() = delete;
    Node(const Data& d, unsigned n) : data(d), nSequence(n), pLeft(nullptr), pParent(nullptr), pRight(nullptr) {}
    Node(const Data& d, unsigned n, Node* pp) : data(d), nSequence(n), pLeft(nullptr), pParent(pp), pRight(nullptr) {}
    Node(const Data& d, unsigned n, Node* pl, Node* pp, Node* pr) : data(d), nSequence(n), pLeft(pl), pParent(pp), pRight(pr) {}
    Node(Data&& d, unsigned n) : data(d), nSequence(n), pLeft(nullptr), pParent(nullptr), pRight(nullptr) {}
    Node(Data&& d, unsigned n, Node* pp) : data(d), nSequence(n), pLeft(nullptr), pParent(pp), pRight(nullptr) {}
    Node(Data&& d, unsigned n, Node* pl, Node* pp, Node* pr) : data(d), nSequence(n), pLeft(pl), pParent(pp), pRight(pr) {}
    ~Node() {} // this Tree implementation does not allocate anything.

    Node*        pLeft;
    Node*        pParent;
    Node*        pRight;
    Data         data;
    unsigned int nSequence;
};

#endif // _NODE_H
