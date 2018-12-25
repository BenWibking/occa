#include <occa/lang/expr/emptyNode.hpp>

namespace occa {
  namespace lang {
    const emptyNode noExprNode;

    emptyNode::emptyNode() :
      exprNode(NULL) {}

    emptyNode::~emptyNode() {}

    udim_t emptyNode::type() const {
      return exprNodeType::empty;
    }

    exprNode* emptyNode::clone() const {
      return new emptyNode();
    }

    void emptyNode::setChildren(exprNodeRefVector &children) {}

    void emptyNode::print(printer &pout) const {}

    void emptyNode::debugPrint(const std::string &prefix) const {
      std::cerr << prefix << "|\n"
                << prefix << "|---o\n"
                << prefix << '\n';
    }
  }
}
