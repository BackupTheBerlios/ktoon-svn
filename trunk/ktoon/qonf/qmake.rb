
require 'info'
require 'extensions'

module RQonf

class QMake
	def initialize
		if not findQMake("4.0.0")
			raise QonfException.new("Can't find valid qmake - qt4")
		end
		
		@make = "make"
	end
	
	def findQMake(minqtversion)
		paths = [ "qmake-qt4", "qmake4", "qmake" ]
		
		close_stderr
		
		minver = minqtversion.split(".")
		paths.each { |path|
			begin
				version = []
				ok = true
				
				IO.popen("#{path} -query QT_VERSION") { |prc|
					version = prc.readlines.join("").split(".")
				}
				
				next if $? != 0
				
				version.size.times { |i|
					if version.size < i and minver.size >= 2
						break
					end
					
					if version[i] < minver[i]
						ok = false
						break
					end 
				}
				
				if ok
					@path = path
					open_stderr
					return ok
				end
			end
		}
		
		open_stderr

		return false
	end
	
	def query(var)
		output = `#{@path} -query #{var}`
		output
	end
	
	def run(args, recur)
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
		IO.popen("#{@make} clean") { |c|
			output = c.readlines.join("")
		}
		
		times = 0
		endcode = 512
		
		while endcode == 512 and times <= 3
			IO.popen("#{@make}", "r") { |c|
				output = c.readlines
			}
			
			endcode = $?
			times += 1
		end
		
		if $? != 0
			return false
		end
		
		return true
	end
end

end #module


