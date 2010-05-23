
require 'qonf/test'
require 'qonf/config'
require 'qonf/info'
require 'qonf/qonfexception'
require 'qonf/makefile'

module RQonf

class Configure
    attr_reader :qmake, :destdir

    def initialize(args)
        #@statusFile = Dir.getwd + "/configure.status"

        @tests = []
        @testsDir = Dir.getwd

        @options = {}
        parseArgs(args)

        @qmake = QMake.new

        @destdir = ""

        if @options['prefix'].nil?
           @destdir = case RQonf::DetectOS.whatOS
                           when 1 # Windows
                                  "/" # FIXME
                           else
                                  "/usr"
                      end
        else
           @destdir = File.expand_path(@options['prefix'])
        end

        #File.open(@statusFile, "w" ) { |file|
        #          file << %@
        #require 'qonf/makefile'
        #RQonf::Makefile::override( ARGV[0].to_s, "#{@destdir}", "#{@statusFile}" )
        #@
        #}
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

        if @qmake.findQMake(minqtversion, true)
           print "[ \033[92mOK\033[0m ]\n"
        else
           print "[FAILED]\n"
           raise QonfException.new("Invalid Qt version.\n   Please, upgrade to #{minqtversion} or higher (Visit: http://www.trolltech.com)")
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

        Info.info << "Updating makefiles..." << $endl

        @makefiles = Makefile::findMakefiles(Dir.getwd)

        @makefiles.each { |makefile|
                   Makefile::override(makefile, @destdir)
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
           Info.warn << "Searching qonf files in: " << path << $endl
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
end


end # module

