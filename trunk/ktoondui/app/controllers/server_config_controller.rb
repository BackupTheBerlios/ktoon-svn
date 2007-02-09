require 'ktoond/serverconfig.rb'
require 'ktoond/serverrunner.rb'

class ServerConfigController < ApplicationController
	before_filter :loadConfig
	layout :determine_layout
	
	def index
		show
		render :action => 'show'
	end
	
	def show
	end
	
	def edit
	end
	
	def update
		config = params[:serverConfig]
		
		if config.nil?
			render :action => 'edit'
			return
		end
		
		@serverConfig.repository = config[:repository]
		@serverConfig.host = config[:host]
		@serverConfig.port = config[:port]
		
		@serverConfig.save
		
		runner = KToonD::Runner.new
		runner.restart
		
		flash[:notice] = i18n("Server updated and running")
		
		redirect_to :action => 'show'
	end
	
	protected
	def createMenu
		menu = {
			i18n("Show") => { :action => 'show' },
			i18n("Edit") => { :action => 'edit' }
			
		}
		
		flash[:sidebar] = menu
	end
	
	private
	def loadConfig
		@serverConfig = KToonD::ServerConfig.new
	end
end
