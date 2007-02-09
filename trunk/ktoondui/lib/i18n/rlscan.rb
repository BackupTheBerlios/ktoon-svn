#!/usr/bin/ruby

$: << File.dirname(__FILE__)

require 'builder/xmlmarkup'
require 'builder/xmlevents'

module I18N

class Scanner
	def initialize(path = nil)
		@toTranslate = []
		
		@path = path
		if path.to_s.empty? or not File.exists?(path)
			@path = "."
		else
			@path = path
		end

	end
	
	def scanStrings
		Dir.foreach(@path) { |path|
			if path =~ /(\.rhtml|\.rb)$/
				File.open(path, "r") { |file|
					file.each_line { |line|
						line.split(",").each { |sentence|
							if sentence =~ /i18n\s*(\(|\s+)\s*\"(.*)\"\s*(\)|\s+)/
								if not $2.empty?
									@toTranslate << $2.strip
								end
							end
						}
					}
				}
			end
		}
	end
	
	def generateTranslateFile
		@toTranslate.uniq!
		xml = Builder::XmlMarkup.new(:indent => 3)
		
		xml.translation :version => 0 do
			@toTranslate.each { |word|
				xml.word do
					xml.source word
					xml.tr ""
				end
			}
		end
		
		File.open("untranslated.rts", "w") { |f|
			f << xml.target! << "\n"
			
			f.close
		}
		
		@toTranslate.clear
	end
	
end

end

if $0 == __FILE__
	scanner = I18N::Scanner.new(ARGV[0])
	scanner.scanStrings
	scanner.generateTranslateFile
	
end


