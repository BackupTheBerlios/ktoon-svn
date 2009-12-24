
require 'qonf/test'
require 'qonf/config'
require 'qonf/info'
require 'qonf/qonfexception'
require 'qonf/makefile'

module RQonf

class Configure
    attr_reader :qmake, :statusFile
    
    def initialize(args)
        @statusFile = Dir.getwd+"/configure.status"
        
        @tests = []
        @testsDir = Dir.getwd
        
        @options = {}
        parseArgs(args)
        Makefile::setArgs(@options)
        
        @qmake = QMake.new
        
        setPath()       

        Makefile::setArgs(@options)
 
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
                        print "[ OK ]\n"
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
        
        #Info.info << "Updating makefiles..." << $endl
        
        @makefiles = Makefile::findMakefiles(Dir.getwd)
        
        @makefiles.each { |makefile|
                           Makefile::override(makefile)
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

   private
    def setPath()
        if @options['prefix'].nil? then
           @options['prefix'] = "/usr/local"
        end
        if @options['bindir'].nil? then
           @options['bindir'] = @options['prefix'] + "/bin" 
        end
        if @options['libdir'].nil? then
           @options['libdir'] = @options['prefix'] + "/lib" 
        end
        if @options['includedir'].nil? then
           @options['includedir'] = @options['prefix'] + "/include" 
        end
        if @options['sharedir'].nil? then
           @options['sharedir'] = @options['prefix'] + "/share"
        end
    end
end
end # module

if __FILE__ == $0
    begin
        conf = RQonf::Configure.new()
        
        conf.verifyQtVersion("4.5.0")
        conf.createTests
        conf.createConfig("config.pri")
    rescue QonfException => err
        Info.error << "Configure failed. error was: #{err.message}\n"
    rescue => err
        Info.error << "General failure: #{err.message}\n" << err.backtrace
    end
end

