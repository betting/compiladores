#include "comp_tree.h"

void preorder(comp_tree_t* p)
{
    if(p==NULL)return;
    printf(" %d",p->data);
    preorder(p->child);

    preorder(p->sibling);
}

comp_tree_t* search(comp_tree_t* root,int data)
{
    if(root==NULL)
        return;
    if(data==root->data)
        return root;
    comp_tree_t* t = search(root->child,data);
    if(t==NULL)
         t = search(root->sibling,data);
    return t;

}

comp_tree_t* createNode(int data)
{
    comp_tree_t* newnode= (comp_tree_t*)malloc(sizeof(comp_tree_t));
    newnode->child=NULL;
    newnode->sibling=NULL;
    newnode->data=data;
    return newnode;
}

comp_tree_t* createnary(comp_tree_t* root,int data[])
{

    //check if node exist already

   comp_tree_t* newnode = search(root,data[0]);
    //if node does not exist
    if(newnode==NULL)
    {
        newnode= createNode(data[0]);
    }

    comp_tree_t* parent=newnode;
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
