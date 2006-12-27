
require 'info'
require 'extensions'

module RQonf

class QMake
	def initialize(path = nil)
		if path.nil?
			paths = [ "qmake-qt4", "qmake4", "qmake" ]
			
			close_stderr
			paths.each { |path|
				begin
					version = []
					
					IO.popen("#{path} -query QT_VERSION") { |prc|
						version = prc.readlines.join("").split(".")
					}
					
					if version.first == "4"
						@path = path
						break
					end
				end
			}
			
			open_stderr
			
		else
			@path = path
		end
		
		if @path.nil?
			raise Exception.exception("Can't find valid qmake - qt4")
		end
		
		@make = "make"
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
		while $? == 512 and times <= 10
			IO.popen("#{@make}", "r") { |c|
				output = c.readlines
			}
			
			times += 1
		end
		
		if $? != 0
			return false
		end
		
		return true
	end
end

end #module


