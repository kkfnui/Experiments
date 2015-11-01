#include <iostream>
#include <assert.h>

typedef struct tagNode{
	int value;
	struct tagNode * left;
	struct tagNode * right;
}Node, *PNode;



typedef PNode TreeRoot;


PNode create_node(int value){
	PNode node = new Node();
	node->value = value;
	node->left = NULL;
	node->right = NULL;
	return node;

}

TreeRoot build_tree(int value){
	return create_node(value);
}

void append(TreeRoot root, int value){
	assert(root != NULL);
	PNode iter = root;
	while (true) {
		if (iter->value <= value){
			if (iter->right != NULL){
				iter = iter->right;
			}else {
				PNode node = create_node(value);
				iter->right = node;
				break;
			}
		}else {
			if(iter->left != NULL){
				iter = iter->left;
			}else {
				PNode node = create_node(value);
				iter->left = node;
				break;
			}
		}
	}
}

void print_tree(TreeRoot root){
	if (root != NULL) {
		printf("%d\n", root->value);
	}
	
	if(root->left != NULL) {
		print_tree(root->left);
		//printf("l-%d\t", root->left->value);
	}
	
	if(root->right != NULL) {
		print_tree(root->right);
		//printf("r-%d\t", root->right->value);

	}
	
}

void print_tt_tree(TreeRoot root){
	
	if (root->left != NULL){
		print_tt_tree(root->left);
	}
	printf("%d\n", root->value);
	
	if (root->right != NULL){
		print_tt_tree(root->right);
	}
	
}


void destory_tree(TreeRoot root){
	
	if (root->left != NULL) {
		destory_tree(root->left);
	}
	
	if (root->right != NULL) {
		destory_tree(root->right);
	}
	
	delete root;
}

void reversal(TreeRoot root){
	if (root->left != NULL){
		reversal(root->left);
	}
	
	if (root->right != NULL) {
		reversal(root->right);
	}
	
	PNode tmp = root->right;
	root->right = root->left;
	root->left = tmp;
}


int main(int argc, char *argv[]) {
	
	TreeRoot root = build_tree(50);
	append(root, 10);
	append(root, 40);
	append(root, 30);
	append(root, 60);
	append(root, 70);
	append(root, 80);
	append(root, 0);
	append(root, 4);
	print_tree(root);
	printf("---------\n");
	print_tt_tree(root);
	reversal(root);
	printf("---------\n");

	print_tt_tree(root);
	destory_tree(root);
}