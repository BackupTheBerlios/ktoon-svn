#!/usr/bin/ruby

require 'qonf/configure'
require 'qonf/info'
require 'qonf/komconfig'

def usage
    puts <<_EOH_
   Use: ./configure [options]
    options:
        --help, -h:                          Show this message
        --prefix=[prefix], -prefix [prefix]: Sets prefix
        --with-debug:                        Enable debug
        --with-ktoondir=[ktoon dir]          Use KToon dir
_EOH_
    exit 0
end


begin
    conf = RQonf::Configure.new(ARGV)

    if conf.hasArgument?("help") or conf.hasArgument?("h")
        usage()
    end
    
    ktoondir = conf.argumentValue("with-ktoondir")
    
    if ktoondir.to_s.empty?
        ktoondir = ENV["KTOON_HOME"]
    end
    
    if ktoondir.to_s.empty?
        usage()
    end

    conf.verifyQtVersion("4.5.0")
    
    ktoondir = File.expand_path(ktoondir)

    config = RQonf::Config.new
    
    config.addModule("core")
    config.addModule("gui")
    config.addModule("svg")
    config.addModule("xml")
    config.addModule("network")

    config.addLib("-lkgui")
    config.addLib("-lkcore")
    config.addLib("-lksound")
    config.addLib("-L#{ktoondir}/lib -lktoon -lstore -lktbase")
    
    config.addLib("-L#{RQonf::CONFIG["libdir"]}")
        config.addIncludePath(RQonf::CONFIG["includepath"])

    config.addIncludePath(ktoondir+"/include/ktoon")
    config.addIncludePath(ktoondir+"/include/store")
    config.addIncludePath(ktoondir+"/include/base")
    
    
    if not conf.hasArgument?("with-debug")
        config.addDefine("D_NODEBUG")
        config.addOption("silent")
    end
    
    unix = config.addScope("unix")
    unix.addVariable("MOC_DIR", ".moc")
    unix.addVariable("UI_DIR", ".ui")
    unix.addVariable("OBJECTS_DIR", ".obj")
    
    config.save("ktoond_config.pri")
    conf.createMakefiles
    
rescue => err
    Info.error << "Configure failed. error was: #{err.message}\n"
    if $DEBUG
        puts err.backtrace
    end
end

