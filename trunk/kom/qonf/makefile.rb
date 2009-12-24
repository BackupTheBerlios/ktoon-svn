
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

        def self.override(makefile, destdir)
            newmakefile = ""
            File.open(makefile, "r") { |f|
                      lines = f.readlines
                      index = 0
                      while index < lines.size
                            line = lines[index]
                            if line.include? "INSTALL_ROOT" then
                               newmakefile += "#{line.gsub( /\$\(INSTALL_ROOT\)/, destdir)}"
                            else
                               newmakefile += line
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
