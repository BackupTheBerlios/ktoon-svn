
require 'rbconfig'

module RQonf
	class Makefile
		def self.findMakefiles(path)
			makefiles = []
			Dir.foreach(path) { |f|
				file = "#{path}/#{f}"
				
				if File.stat(file).directory?
					if not f =~ /^\./
						makefiles.concat findMakefiles(file)
					end
				elsif f.downcase == "makefile"
					makefiles << file
				end
			}
			
			makefiles
		end
		
		def self.override(makefile, destdir, statusFile)
			newmakefile = ""
			
			ruby_bin = "#{::Config::CONFIG["bindir"]}/#{::Config::CONFIG["ruby_install_name"]}"
			
			File.open(makefile, "r") { |f|
				lines = f.readlines
				
				index = 0
				while index < lines.size
					line = lines[index]
					
					if line =~ /^\s+\$\(QMAKE\)/
						newmakefile += line
						newmakefile += "\t#{ruby_bin} #{statusFile} #{makefile}\n"
						index += 1
					else
						newmakefile += "#{line.gsub( /\$\(INSTALL_ROOT\)/, destdir )}"
					end
					
					index += 1
				end
			}
			
			File.open(makefile, "w") { |f|
				f << newmakefile
			}
		end
	end
end


