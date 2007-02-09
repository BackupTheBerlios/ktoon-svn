require File.dirname(__FILE__) + '/../test_helper'
require 'mainpage_controller'

# Re-raise errors caught by the controller.
class MainpageController; def rescue_action(e) raise e end; end

class MainpageControllerTest < Test::Unit::TestCase
  def setup
    @controller = MainpageController.new
    @request    = ActionController::TestRequest.new
    @response   = ActionController::TestResponse.new
  end

  # Replace this with your real tests.
  def test_truth
    assert true
  end
end
