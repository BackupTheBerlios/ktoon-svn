
module QTestGenerator

class Engine
	def initialize(path = nil)
		@files = []
		
		if path.to_s.empty? or not File.exists?(path)
			@path = "."
		else
			@path = path
		end
	end
	
	def scanCpp
		print "Scanning files... "
		@files.clear
		
		Dir.foreach(@path) { |path|
			if path =~ /.cpp$/
				File.open(path, "r") { |file|
					ok = false
					file.each_line { |line|
						if line =~ /^\s*int\s+main\s*\(/
							@files << path
							ok = true
						elsif line =~ /^\s*QTEST_MAIN\s*\(\s*\w+\s*\)/
							@files << path
							ok = true
						end
						
						break if ok
					}
					
					if not ok
						$stderr << "=> Invalid file: #{path}\n"
					end
				}
			end
		}
		
		puts "OK"
		
		@files
	end
	
	def generate
		puts "Generating projects... "
		@files.each { |path|
			generatePro(path)
		}
	end
	
	def compile
		puts "Compiling... "
		@files.each { |path|
			project = File.basename(path, ".cpp")+".pro"
			`qmake #{project}`
			`make clean 2>/dev/null`
			`make`
		}
	end
	
	def execute
		puts "Executing tests... "
		@files.each { |path|
			bin = File.basename(path, ".cpp")
			puts `./#{bin}`
		}
	end
	
	def clean
		@files.each { |path|
			project = File.basename(path, ".cpp")+".pro"
			`qmake #{project}`
			`make distclean`
		}
	end
	
	def run
		scanCpp
		generate
		compile
		execute
	end
	
	private
	def generatePro(path)
		endl = "\n"
		basename = File.basename(path, ".cpp")
		File.open(basename+".pro", "w") { |f|
			f << "TEMPLATE = app" << endl
			f << "TARGET = #{basename}" << endl
			f << "DEPENDPATH += ." << endl
			f << "INCLUDEPATH += ." << endl
			f << "CONFIG += qtestlib" << endl
			f << "SOURCES += #{path}" << endl
			f << "include(tests_config.pri)" << endl
		}
	end
end


end


if __FILE__ == $0
	engine = QTestGenerator::Engine.new(ARGV[0])
	engine.run
	
	ARGV.each { |arg|
		if arg =~ /-c/
			engine.clean
		end
	}
end

