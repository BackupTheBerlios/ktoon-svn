
require 'qonf/info'
require 'qonf/extensions'

module RQonf

class QMake

    $found = ""

    def initialize
        if not findQMake("4.0.0",false)
           raise QonfException.new("Can't find valid qmake - qt4")
        end

        @make = "make"
    end

    # This method check if the current version of Qt is valid for KToon compilation	
    def findQMake(minqtversion, verbose)
        close_stderr
        qtversion = ""
        paths = [ "qmake", "qmake-qt4", "qmake4" ]
        minver = minqtversion.split(".")
        flag = true

        paths.each { |path|
                     begin
                         version = []
                         sites = []
                         distance = 0

                         IO.popen("whereis #{path}") { |result|
                                  sites = result.readlines.join("").split(":")
                                  word = sites[1].chop
                                  distance = word.length
                         }

                         if distance > 0
                            IO.popen("#{path} -query QT_VERSION") { |prc|
                                     found = prc.readlines.join("")
                                     version = found.split(".")
                                     if (found.length != 0)
                                         qtversion = found.chop
                                     end
                            }
                            next if $? != 0

                            version.size.times { |i|
                                    if version.size < i and minver.size >= 2
                                       break
                                    end

                                    if version[i] < minver[i]
                                       flag = false
                                       break
                                    end 
                            }

                            if flag 
                               @path = path
                               open_stderr
                               break	
                            end
                         end
                     end
        }
        open_stderr

        if verbose 
           print "(Found: #{qtversion}) "
        end

        return flag
    end

    def query(var)
        output = `#{@path} -query #{var}`
        output
    end

    def run(args = "", recur = false)
        options = ""
        if recur
           options += "-r"
        end

        output = `#{@path} #{args} #{options} `

        if output.strip.empty?
           return true
        end

        return false
    end

    def compile
        fork do
             IO.popen("#{@make} clean  2>/dev/null") { |c|
                output = c.readlines
             }

             times = 0
             endcode = 2

             while endcode == 2 and times <= 3
                   IO.popen("#{@make} 2>/dev/null", "r") { |c|
                            output = c.readlines
                   }
                   endcode = $? >> 8
                   times += 1
             end

             exit -1 if endcode != 0
        end

        Process.wait

        if $? != 0
           return false
        end

        return true
    end

end

end #module
