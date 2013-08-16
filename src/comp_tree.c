#include "comp_tree.h"

void preorder(Tree* p)
{
    if(p==NULL)return;
    printf(" %d",p->data);
    preorder(p->child);

    preorder(p->sibling);
}

Tree* search(Tree* root,int data)
{
    if(root==NULL)
        return;
    if(data==root->data)
        return root;
    Tree* t = search(root->child,data);
    if(t==NULL)
         t = search(root->sibling,data);
    return t;

}

Tree* createNode(int data)
{
    Tree* newnode= (Tree*)malloc(sizeof(Tree));
    newnode->child=NULL;
    newnode->sibling=NULL;
    newnode->data=data;
    return newnode;
}

Tree* createnary(Tree* root,int data[])
{

    //check if node exist already

   Tree* newnode = search(root,data[0]);
    //if node does not exist
    if(newnode==NULL)
    {
        newnode= createNode(data[0]);
    }

    Tree* parent=newnode;
    /////now create node of its children
    int j;
    for(j=0;j<data[1];j++)
    {
        //for first child
        if(j==0)
        {
             parent->child=createNode(data[j+2]);
             parent = parent->child;
        }
        //for all other childs
        else
        {
            parent->sibling=createNode(data[j+2]);
            parent = parent->sibling;
        }

    }

    if(root==NULL)
            root = newnode;
    return root;


}
