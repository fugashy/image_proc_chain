// Copyright 2019 fugashy
#ifndef IMAGE_PROC_CHAIN__CHAIN_PROCESSOR_HPP_
#define IMAGE_PROC_CHAIN__CHAIN_PROCESSOR_HPP_
#include <memory>

#include "image_proc_chain/flexible_chain_executor.hpp"
#include "image_proc_chain/srv/change_chain_num.hpp"


namespace image_proc_chain {

class ChainProcessor {
 public:
  /**
   * @brief Initialize FlexibleChainExecutor and advertise service
   *
   * @param[in] node Pointer to node
   */
  explicit ChainProcessor(rclcpp::Node::SharedPtr& node);

  /**
   * @brief Undeclare parameters
   */
  virtual ~ChainProcessor();

  /**
   * @brief Get pointer of executor
   *
   * ex) get_executor()->spin();
   *
   * @return Pointer to executor
   */
  rclcpp::executors::SingleThreadedExecutor::SharedPtr get_executor() const {
    return flexible_chain_executor_->get_executor();
  }

 private:
  /**
   * @brief Change number of chain
   *
   * Binded as callback function for advertised service
   *
   * @param[in] request_header Pointer to request header
   * @param[in] request Pointer to request
   * @param[out] response Pointer to response
   */
  void ChangeNumberOfChain(
      const std::shared_ptr<rmw_request_id_t> request_header,
      const std::shared_ptr<srv::ChangeChainNum::Request> request,
      std::shared_ptr<srv::ChangeChainNum::Response> response);

  /**
   * @brief Pointer to node
   */
  rclcpp::Node::SharedPtr node_;

  /**
   * @brief Pointer to FlexibleChainExecutor
   */
  FlexibleChainExecutor::SharedPtr flexible_chain_executor_;

  /**
   * @brief Pointer to service server
   */
  rclcpp::Service<srv::ChangeChainNum>::SharedPtr srv_;
};

}  // namespace image_proc_chain

#endif  // IMAGE_PROC_CHAIN__CHAIN_PROCESSOR_HPP_
