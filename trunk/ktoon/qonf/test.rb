
require 'qmake'
require 'rexml/parsers/sax2parser'
require 'rexml/sax2listener'

require 'info'

module RQonf

class Test
	include REXML
	
	attr_reader :rules
	
	def initialize(rulesPath)
		@rules = rulesPath
		@qmake = QMake.new
	end
	
	def run(config)
		parser = Parser.new
		
		return false if not parser.parse(@rules) or parser.name.empty?
		
		Info.info << "Checking for " << parser.name << "... "
		
		dir = "#{File.dirname(@rules)}/#{parser.dir}"
		
		if $DEBUG
			Info.warn << "Running in " << dir << $endl
		end
		
		cwd = Dir.getwd
		if File.exists?(dir)
			if File.stat(dir).directory?
				Dir.chdir(dir)
				@qmake.run( "'INCLUDEPATH += #{parser.includes.join(" ")}' 'LIBS += #{parser.libs.join(" ")}'" ,true)
				if not @qmake.compile
					Dir.chdir(cwd)
					print "[ FAIL ]\n"
					return false
				end
			else
				Dir.chdir(cwd)
				print "[ FAIL ]\n"
				return false
			end
		else
			Dir.chdir(cwd)
			print "[ FAIL ]\n"
			return false
		end
		
		Dir.chdir(cwd)
		
		parser.includes.each { |inc|
			config.addIncludePath(inc)
		}
		
		parser.libs.each { |lib|
			config.addLib(lib)
		}
		
		parser.defines.each { |define|
			config.addDefine(define)
		}
		
		print "[ OK ]\n"
		
		return true
	end
	
	private
	class Parser
		include REXML::SAX2Listener
		
		attr_reader :name
		attr_reader :dir
		attr_reader :defines
		attr_reader :includes
		attr_reader :libs
		
		def initialize
			@name = ""
			@current_tag = ""
			@section = ""
			@dir = ""
			@defines = []
			@includes = []
			@libs = []
		end
		
		def start_document
		end
		
		def end_document
		end
		
		def start_element( uri, localname, qname, attributes)
			
			case qname
				when "test"
					@section = qname
					@name = attributes["name"].to_s
				when "dir"
					if @section == "test"
						@dir = attributes["path"]
					end
				when "provide"
					@section = qname
				when "define"
					if @section  == "provide"
						@defines << attributes["value"]
					end
				when "include"
					if @section  == "provide"
						@includes << attributes["path"]
					end
				when "lib"
					if @section  == "provide"
						@libs << attributes["path"]
					end
				when "command"
					if @section == "provide"
						if @current_tag == "includes"
							IO.popen(attributes["value"]) { |c|
								output = c.readlines.join("").split(" ")
								
								output.each { |i|
									if i =~ /-I(.*)/
										@includes << $1.chomp
									elsif not i =~ /-.*/
										@includes << i
									end
								}
							}
						elsif @current_tag == "libs"
							IO.popen(attributes["value"]) { |c|
								output = c.readlines.join("").split(" ")
								
								output.each { |lib|
									if lib =~ /-l(.*)/
										@libs << lib
									elsif lib[0].chr != "-"
										@libs << lib
									end
								}
								@libs.concat(output)
							}
						end
					end
			end
			
			@current_tag = qname
		end
		
		def end_element( uri, localname, qname)
			case qname
				when ""
			end
			
		end
		
		def characters(text)
			text = text.strip
			if not text.empty?
			end
		end
		
		def parse(path)
			sax2 = REXML::Parsers::SAX2Parser.new( File.new( path ) )
			sax2.listen(self)
			
			begin
				sax2.parse
			rescue
				return false
			end
			
			return true
		end
	end
end

end #module

