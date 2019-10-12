#include "image_proc_chain/image_processors.hpp"

#include <chrono>

#include <opencv2/imgproc.hpp>

using std::placeholders::_1;

namespace image_proc_chain {
namespace image_processors {

Base::Base(std::shared_ptr<rclcpp::Node>& node) : node_(node) {}

GaussianSpacial::GaussianSpacial(std::shared_ptr<rclcpp::Node>& node)
    : Base(node),
      kernel_(cv::Size(21, 21)),
      sigma_(cv::Vec2d(1.0, 1.0)),
      iteration_count_(1)  {
  node->set_on_parameters_set_callback(std::bind(&GaussianSpacial::ChangeParameters, this, _1));
  node->declare_parameter("~/kernel_x", 21);
  node->declare_parameter("~/kernel_y", 21);
  node->declare_parameter("~/sigma_x", 1.0);
  node->declare_parameter("~/sigma_y", 1.0);
  node->declare_parameter("~/iteration_count", 1);
}

GaussianSpacial::~GaussianSpacial() {
  node_->declare_parameter("~/kernel_x");
  node_->declare_parameter("~/kernel_y");
  node_->declare_parameter("~/sigma_x");
  node_->declare_parameter("~/sigma_y");
  node_->declare_parameter("~/iteration_count");
}

cv::Mat GaussianSpacial::Process(const cv::Mat& image_in) {
  cv::Mat tmp = image_in;
  cv::Mat image_out;
  for (int i = 0, iend = iteration_count_; i < iend; ++i) {
    cv::GaussianBlur(
        tmp, image_out, kernel_, sigma_[0], sigma_[1]);
    tmp = image_out.clone();
  }

  return image_out;
}

rcl_interfaces::msg::SetParametersResult GaussianSpacial::ChangeParameters(
    const std::vector<rclcpp::Parameter>& params) {
  auto result = rcl_interfaces::msg::SetParametersResult();
  result.successful = true;

  for (auto param : params) {
    if (param.get_name() == "~/kernel_x") {
      const int width = param.as_int();
      if (width % 2 != 1 or width < 1 or width % 2 != 1 or width < 1) {
        result.successful = false;
      } else {
        kernel_.width = param.as_int();
      }
    }
    if (param.get_name() == "~/kernel_y") {
      const int height = param.as_int();
      if (height % 2 != 1 or height < 1 or height % 2 != 1 or height < 1) {
        result.successful = false;
      } else {
        kernel_.height = param.as_int();
      }
    }
    if (param.get_name() == "~/sigma_x") {
      sigma_[0] = param.as_double();
    }
    if (param.get_name() == "~/sigma_y") {
      sigma_[1] = param.as_double();
    }
    if (param.get_name() == "~/iteration_count") {
      iteration_count_ = param.as_int();
    }
  }
  RCLCPP_INFO(node_->get_logger(), "parameter has changed");

  return result;
}

}
}