require File.dirname(__FILE__) + '/../test_helper'
require 'server_config_controller'

# Re-raise errors caught by the controller.
class ServerConfigController; def rescue_action(e) raise e end; end

class ServerConfigControllerTest < Test::Unit::TestCase
  def setup
    @controller = ServerConfigController.new
    @request    = ActionController::TestRequest.new
    @response   = ActionController::TestResponse.new
  end

  # Replace this with your real tests.
  def test_truth
    assert true
  end
end
