
require 'rexml/parsers/sax2parser'
require 'rexml/sax2listener'

require 'builder.rb'

module KToonD

class ServerConfigParser
	include REXML::SAX2Listener

	def initialize
		reset
	end

	def reset
		@current_tag = ""
		@key = ""
		
		@parsing = false
		@readText = false
		@ignore = false

		@data = {}
	end

	def start_document
		reset
	end

	def end_document
	end

	def start_element( uri, localname, qname, attributes)
		return if @ignore
		
		if not @parsing
			if qname == "DConfig"
				@parsing = true
			else
				raise "Invalid input"
			end
		end
		
		case qname
			when 'host'
				@data[:host] = attributes["value"]
			when 'port'
				@data[:port] = attributes["value"].to_i
			when 'repository'
				@data[:repository] = attributes["value"]
		end

		@current_tag = qname
	end

	def end_element( uri, localname, qname)
	end

	def characters(text)
		if @readText
			@readText = false
		end
	end

	def parseFile(path)
		reset
		
		parser = REXML::Parsers::SAX2Parser.new( File.new( path ) )
		
		parser.listen(self)
		parser.parse
		@data
	end

	def parse(doc)
		reset
		parser = REXML::Parsers::SAX2Parser.new( doc )
		parser.listen(self)
		parser.parse
		@data
	end
end

class ServerConfigDocument
	def initialize(values)
		@xml = Builder::XmlMarkup.new(:indent => 3)
		@xml.DConfig do
			@xml.Connection do
				@xml.host :value => values[:host]
				@xml.port :value => values[:port]
			end
			@xml.General do
				@xml.repository :value => values[:repository]
			end
		end
	end
	
	def to_s
		@xml.target!
	end
end

class ServerConfig
	attr_accessor :binpath
	attr_accessor :host
	attr_accessor :port
	attr_accessor :repository
	
	def initialize(path = nil)
		@errors = {}
		@binpath = "ktoond"
		
		if path.to_s.empty?
			path = ENV["HOME"]+"/.ktoond/ktoond.cfg"
		end
		
		loadFrom(path)
	end
	
	
	def loadFrom(path)
		parser = ServerConfigParser.new
		data = parser.parseFile(path)
		
		@host = data[:host]
		@port = data[:port]
		@repository = data[:repository]
		
		@path = path
	end
	
	def save
		doc = ServerConfigDocument.new({:host => @host, :port => @port, :repository => @repository})
		
		File.open(@path, "w") { |f|
			f << doc.to_s << "\n"
		}
	end
end

end

