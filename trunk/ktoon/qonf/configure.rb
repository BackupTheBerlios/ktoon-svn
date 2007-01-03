
require 'test'
require 'config'
require 'info'
require 'qonfexception'

module RQonf

class Configure
	def initialize(args)
		@tests = []
		@testsDir = Dir.getwd
		
		@options = {}
		parseArgs(args)
		
		@qmake = QMake.new
	end

	def hasArgument?(arg)
		@options.has_key?(arg)
	end

	def argumentValue(arg)
		@options[arg].to_s
	end
	
	def setTestDir(dir)
		@testsDir = dir
	end
	
	def verifyQtVersion(minqtversion)
		Info.info << "Checking for Qt >= " << minqtversion << "... "
		
		if @qmake.findQMake(minqtversion)
			print "[OK]\n"
		else
			print "[FAILED]\n"
			raise QonfException.new("Invalid Qt version")
		end
	end
	
	def createTests
		@tests.clear
		findTest(@testsDir)
	end
	
	def runTests(config)
		@tests.each { |test|
			if not test.run(config) and not test.optional
				raise QonfException.new("Required")
			end
		}
	end
	
	def createMakefiles
		Info.info << "Creating makefiles..." << $endl
		@qmake.run("", true)
		
		return if @options['prefix'].nil?
		
		
		Info.info << "Updating makefiles..." << $endl
		
		@makefiles = []
		findMakefiles(Dir.getwd)
		
		@makefiles.each { |makefile|
			newmakefile = ""
			File.open(makefile, "r") { |f|
				lines = f.readlines
				
				lines.each { |line|
					newmakefile += "#{line.gsub( /\$\(INSTALL_ROOT\)/, @options['prefix'])}"
				}
			}
			
			File.open(makefile, "w") { |f|
				f << newmakefile
			}
		}
	end
	
	private
	def parseArgs(args)
		optc = 0
		last_opt = ""
		while args.size > optc
			arg = args[optc].strip
			
			if arg =~ /^--([\w-]*)={0,1}([\W\w]*)/
				opt = $1.strip
				val = $2.strip

				@options[opt] = val
				
				last_opt = opt
			elsif arg =~ /^-(\w)/
				@options[$1.strip] = nil
				last_opt = $1.strip
			else
				# arg is an arg for option
				if not last_opt.to_s.empty? and @options[last_opt].to_s.empty?
					@options[last_opt] = arg
				else
					raise "Invalid option: #{arg}"
				end
			end
		
			optc += 1
		end
	end
	
	def findTest(path)
		if $DEBUG
			Info.warn << "Searching qonfs in: " << path << $endl
		end
		Dir.foreach(path) { |f|
			
			file = "#{path}/#{f}"
			
			if File.stat(file).directory?
				if not f =~ /^\./
					findTest(file)
				end
			elsif file =~ /.qonf$/
				@tests << Test.new(file, @qmake)
			end
		}
	end
	
	def findMakefiles(path)
		Dir.foreach(path) { |f|
			file = "#{path}/#{f}"
			
			if File.stat(file).directory?
				if not f =~ /^\./
					findMakefiles(file)
				end
			elsif f.downcase == "makefile"
				@makefiles << file
			end
		}
	end
end


end # module

if __FILE__ == $0
	begin
		conf = RQonf::Configure.new()
		
		conf.verifyQtVersion("4.2.0")
		conf.createTests
		conf.createConfig("config.pri")
	rescue QonfException => err
		Info.error << "Configure failed. error was: #{err.message}\n"
	rescue => err
		Info.error << "General failure: #{err.message}\n" << err.backtrace
	end
end

