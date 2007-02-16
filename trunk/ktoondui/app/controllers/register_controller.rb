require 'rexml/document'
require 'ktoond/serverconfig.rb'

class RegisterController < ApplicationController
	def index
	end
	
	def register
		reg = params[:reg]
		
		if reg.nil?
			redirect_to :action => 'index'
			return
		end
		
		
		if saveReg(reg)
			flash[:notice] = i18n("Register completed!")
			redirect_to :controller => 'mainpage', :action => 'index'
		else
			flash[:notice] = i18n("Can't save yout form")
			redirect_to :action => 'index'
		end
	end
	
	protected
	def createMenu
		flash[:sidebar] = nil
	end
	
	def saveReg(reg)
		serverConfig = KToonD::ServerConfig.new
		
		path = serverConfig.repository+"/../database/petitions.xml" # FIXME FIXME FIXME
		
		if not File.exists?(path)
			doc = REXML::Document.new
			doc.add_element "registerlist"
			
			File.open(path, "w") { |f|
				f << doc
				f.close
			}
		end
		
		f = File.new(path, "r")
		doc = REXML::Document.new(f)
		f.close
		
		element = doc.root.add_element('register')
		element.add_element("login", { "value" => reg[:login] })
		element.add_element("name", { "value" => reg[:name]})
		element.add_element("email", { "value" => reg[:email]})
		element.add_element("comments", { "value" => reg[:comments]})
		
		begin
			docstr = ""
			doc.write(docstr, 0, false)
			
			File.open(path, "w") { |f|
				f << docstr << "\n"
			}
		rescue
			return false
		end
		
		true
	end
end

