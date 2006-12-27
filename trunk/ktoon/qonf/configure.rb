
require 'test'
require 'config'
require 'info'

module RQonf

class Configure
	def initialize(args)
		@tests = []
		@testsDir = Dir.getwd
		
		@options = {}
		parseArgs(args)
		
	end
	
	def setTestDir(dir)
		@testsDir = dir
	end
	
	def verifyQtVersion(minqtversion)
		Info.info << "Checking for Qt >= " << minqtversion << "... "
		
		ok = true
		begin
			qmake = QMake.new
			minver = minqtversion.split(".")
			qtver = qmake.query("QT_VERSION").split(".")
			
			qtver.size.times { |i|
				if minver.size < i and minver.size >= 2
					break
				end
				
				if qtver[i] < minver[i]
					ok = false
					break
				end 
			}
		rescue
			ok = false
		end
		
		if ok
			print "[OK]\n"
		else
			print "[FAILED]\n"
			
			raise Exception.exception("Invalid Qt version")
		end
	end
	
	def createTests
		@tests.clear
		findTest(@testsDir)
	end
	
	def createConfig(path)
		config = RQonf::Config.new(path)
		
		
		@tests.each { |test|
			if not test.run(config)
				puts "Fallo #{test.rules}"
			end
		}
		
		config
	end
	
	def createMakefiles
		qmake = QMake.new
		
		Info.info << "Creating makefiles..." << $endl
		qmake.run("", true)
		
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
			
			if arg =~ /^--(\w*)={0,1}([\W\w]*)/
				opt = $1
				val = $2
				
				@options[opt] = val
				
				last_opt = opt
			elsif arg =~ /^-(\w)/
				@options[$1] = nil
				last_opt = $1
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
				@tests << Test.new(file)
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
	rescue => err
		Info.error << "Configure failed. error was: #{err.message}\n"
	end
end

