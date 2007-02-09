
require 'rexml/parsers/sax2parser'
require 'rexml/sax2listener'

module I18N

class XmlReader
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

		@words = []
	end

	def start_document
		reset
	end

	def end_document
	end

	def start_element( uri, localname, qname, attributes)
		return if @ignore
		
		qname.downcase!
		
		if not @parsing
			if qname == "translation"
				@parsing = true
			else
				raise "Invalid input"
			end
		end
		
		case qname
			when 'word'
				@words << {}
			when 'source'
				@readText = true
			when 'tr'
				@readText = true
		end

		@current_tag = qname
	end

	def end_element( uri, localname, qname)
	end

	def characters(text)
		if @readText
			text.strip!
			if not text.empty?
				case @current_tag
					when 'source'
						@words.last[:source] = text
					when 'tr'
						@words.last[:tr] = text
				end
			end
			
			@readText = false
		end
	end

	def parseFile(path)
		reset
		
		parser = REXML::Parsers::SAX2Parser.new( File.new( path ) )
		
		parser.listen(self)
		parser.parse
		@words
	end

	def parse(doc)
		reset
		parser = REXML::Parsers::SAX2Parser.new( doc )
		parser.listen(self)
		parser.parse
		@words
	end
end


end


