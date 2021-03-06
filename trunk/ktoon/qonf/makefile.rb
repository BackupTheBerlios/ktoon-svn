
require 'rbconfig'

module RQonf
    class Makefile

        def self.setArgs(paths)
            @options = paths 
        end

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
        
        def self.override(makefile)
            newmakefile = ""
            File.open(makefile, "r") { |f|
                      lines = f.readlines
                      index = 0
                      while index < lines.size
                            line = lines[index]
                            if line.include? "INSTALL_ROOT" then
                               if line.include? "\$\(INSTALL_ROOT\)\/plugins" then
                                  newmakefile += "#{line.gsub(/\$\(INSTALL_ROOT\)\/plugins/, @options['sharedir']+"/plugins")}"
                               elsif line.include? "\$\(INSTALL_ROOT\)\/lib" then
                                     newmakefile += "#{line.gsub(/\$\(INSTALL_ROOT\)\/lib/, @options['libdir'])}"
                               elsif line.include? "\$\(INSTALL_ROOT\)\/include" then
                                     newmakefile += "#{line.gsub(/\$\(INSTALL_ROOT\)\/include/, @options['includedir'])}"
                               elsif line.include? "\$\(INSTALL_ROOT\)\/data" then
                                     newmakefile += "#{line.gsub(/\$\(INSTALL_ROOT\)\/data/, @options['sharedir']+"/data")}"
                               elsif line.include? "\$\(INSTALL_ROOT\)\/bin" then
                                     newmakefile += "#{line.gsub(/\$\(INSTALL_ROOT\)\/bin/, @options['bindir'])}"
                               elsif line.include? "\$\(INSTALL_ROOT\)\/themes" then
                                     newmakefile += "#{line.gsub(/\$\(INSTALL_ROOT\)\/themes/, @options['sharedir']+"/themes")}"
                               elsif line.include? "\$\(INSTALL_ROOT\)\/applications" then
                                     newmakefile += "#{line.gsub(/\$\(INSTALL_ROOT\)\/applications/, "/usr/share/applications")}"
                               elsif line.include? "\$\(INSTALL_ROOT\)\/pixmaps" then
                                     newmakefile += "#{line.gsub(/\$\(INSTALL_ROOT\)\/pixmaps/, "/usr/share/pixmaps")}"
                               end
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


