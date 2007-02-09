# Filters added to this controller apply to all controllers in the application.
# Likewise, all the methods added will be available for all controllers.

class ApplicationController < ActionController::Base
	# Pick a unique cookie name to distinguish our session data from others'
	session :session_key => '_ktoondui_session_id'
	before_filter :createMenu
	layout :determine_layout
	
	def determine_layout
		"default"
	end
	
	protected
	def createMenu
		raise NotImplementedError
	end
end
