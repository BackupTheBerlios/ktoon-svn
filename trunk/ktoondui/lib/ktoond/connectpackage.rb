
require 'builder'

module KToonD

class ConnectPackage
	def initialize(login, password)
		@xml = Builder::XmlMarkup.new
		
		@xml.connect :version => 0 do
			@xml.client 1
			@xml.login login
			@xml.password password
		end
	end
	
	def to_s
		@xml.target!
	end
end

end



