#include "node.h"

Node::Node()
{
    this->type = NoType;
    this->statu = NoStaru;
    this->str = QString();
    parent = 0;
}

Node::Node(NodeType type, Statu statu, const QString &str, int num)
{
    this->type = type;
    this->statu = statu;
    this->str = str;
    this->num = num;
    parent = 0;
}

bool Node::addChild(Node *node)
{
    if(!node)
        return false;
    childen.prepend(node);
    node->parent = this;
}

Node::~Node()
{
    //qDeleteAll(childen);
}
