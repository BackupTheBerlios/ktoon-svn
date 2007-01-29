require 'ktoon/connection'
require 'ktoon/connectpackage'

require 'md5'

class MainController < ApplicationController
	layout :determine_layout
	
	def index
		@cnx = KToon::Connection.new("localhost", 6502)
		@cnx.run
		
		pkg = KToon::ConnectPackage.new("krawek", MD5.md5(""))
		
		@cnx.add_observer(self)
		
		@cnx.sendToServer(pkg)
	end
	
	def update(doc)
		session[:remove] = doc
	end
end
