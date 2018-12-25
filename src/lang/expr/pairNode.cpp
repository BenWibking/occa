#include <occa/lang/expr/pairNode.hpp>

namespace occa {
  namespace lang {
    pairNode::pairNode(operatorToken &opToken,
                       const exprNode &value_) :
      exprNode(&opToken),
      op(*(opToken.op)),
      value(value_.clone()) {}

    pairNode::pairNode(const pairNode &node) :
      exprNode(node.token),
      op(node.op),
      value(node.value->clone()) {}

    pairNode::~pairNode() {
      delete value;
    }

    udim_t pairNode::type() const {
      return exprNodeType::pair;
    }

    opType_t pairNode::opType() const {
      return op.opType;
    }

    exprNode* pairNode::startNode() {
      return value->startNode();
    }

    exprNode* pairNode::endNode() {
      return value->endNode();
    }

    exprNode* pairNode::clone() const {
      return new pairNode(token->to<operatorToken>(),
                          *value);
    }

    bool pairNode::canEvaluate() const {
      token->printError("[Waldo] (pairNode) Unsure how you got here...");
      return false;
    }

    primitive pairNode::evaluate() const {
      token->printError("[Waldo] (pairNode) Unsure how you got here...");
      return primitive();
    }

    void pairNode::setChildren(exprNodeRefVector &children) {
      children.push_back(&value);
    }

    void pairNode::print(printer &pout) const {
      token->printError("[Waldo] (pairNode) Unsure how you got here...");
    }

    void pairNode::debugPrint(const std::string &prefix) const {
      printer pout(std::cerr);
      std::cerr << prefix << "|\n"
                << prefix << "|---[";
      pout << op;
      std::cerr << "] (pairNode)\n";
      value->childDebugPrint(prefix);
    }
  }
}
