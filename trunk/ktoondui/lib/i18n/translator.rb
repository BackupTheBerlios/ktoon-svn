
require 'i18n/xmlreader.rb'

module I18N

class Translator
	def initialize
		@words = {}
	end
	
	def loadFile(path)
		reader = XmlReader.new
		words = reader.parseFile(path)
		
		words.each { |word|
			@words[word[:source]] = word[:tr]
		}
	end
	
	def traduction(word)
		if @words.has_key?(word)
			return @words[word]
		end
		
		return word
	end
end

end

if $translator.nil?
	$translator = I18N::Translator.new
	$translator.freeze
end

module Kernel
	def i18n(word)
		$translator.traduction(word)
	end
end



