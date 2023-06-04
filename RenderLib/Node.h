#pragma once
#include "stdafx.h"
#include "UpdateCallback.h"
class Visitor;

// abstract base class for a scene graph node
class Node
{
	std::vector<Node*> children;
	UpdateCallback* callback = nullptr;
public:
	void AddChild(Node* c) { children.push_back(c); }
	Node* GetChild(int i) { return children[i]; }
	int GetNumChildren() { return children.size(); }
	void SetUpdateCallback(UpdateCallback* cb ) { callback = cb; }
	UpdateCallback* GetUpdateCallback() const { return callback;  }
	virtual void Accept(Visitor* v) = 0;
};