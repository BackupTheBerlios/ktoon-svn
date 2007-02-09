class MainpageController < ApplicationController
	def index
		flash[:sidebar] = nil
	end
	
	protected
	def createMenu
	end
end

