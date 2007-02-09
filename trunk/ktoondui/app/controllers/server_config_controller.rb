require 'ktoond/serverconfig.rb'

class ServerConfigController < ApplicationController
	layout :determine_layout
	
	def index
		show
		render :action => 'show'
	end
	
	def show
		@serverConfig = KToonD::ServerConfig.new
	end
	
	def create
	end
	
	def edit
	end
	
	
end
