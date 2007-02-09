require 'ktoond/serverconfig.rb'

class ServerConfigController < ApplicationController
	before_filter :loadConfig
	layout :determine_layout
	
	def index
		show
		render :action => 'show'
	end
	
	def show
	end
	
	def create
	end
	
	def edit
	end
	
	def update
		config = params[:config]
		
		@serverConfig.repository = config[:repository]
		@serverConfig.host = config[:host]
		@serverConfig.port = config[:port]
		
		@serverConfig.save
		
		redirect_to :action => 'show'
	end
	
	
	private
	def loadConfig
		@serverConfig = KToonD::ServerConfig.new
	end
end
