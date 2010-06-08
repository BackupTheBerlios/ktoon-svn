#!/usr/bin/ruby

# TODO: This script must detect if every command line given is right
#       Currently, it just try to check if some of them are included or not

require 'qonf/configure'
require 'qonf/info'
require 'qonf/komconfig'

begin
    conf = RQonf::Configure.new(ARGV)

    if conf.hasArgument?("help") or conf.hasArgument?("h")
        puts <<_EOH_
Use: ./configure [options]
    options:
        --help, -h:                             Show this message
        --prefix=[path], -prefix [path]:        Sets installation path [/usr/local]
        --bindir=[path], -bindir [path]:        Set binaries path [/usr/local/bin]
        --libdir=[path], -libdir [path]:        Set library path [/usr/local/lib]
        --includedir=path], -includedir [path]: Set include path [/usr/local/include]
        --sharedir=path], -share [path]:        Set data path [/usr/local/share]
        --with-debug:                           Enable debug
_EOH_
        exit 0
    end

    conf.setTestDir("configure.tests")
    conf.verifyQtVersion("4.6.2")
    conf.createTests
    
    config = RQonf::Config.new
    
    conf.runTests(config)

    config.addModule("core")
    config.addModule("gui")
    config.addModule("svg")
    config.addModule("xml")
    config.addModule("network")

    config.addLib("-lkgui")
    config.addLib("-lkcore")
    config.addLib("-lksound")
    config.addLib("-lz")
    
    config.addLib("-L#{RQonf::CONFIG["libdir"]}")
    config.addIncludePath(RQonf::CONFIG["includepath"])
    
    config.addDefine('VERSION=\\\\\"0.9a\\\\\"')
    config.addDefine('CODE_NAME=\\\\\"Phoenix\\\\\"')
    config.addDefine('REVISION=\\\\\"1499\\\\\"')

        Info.info << "Debug support... "

        if conf.hasArgument?("with-debug")
           config.addDefine("K_DEBUG")
           print "[ \033[92mON\033[0m ]\n"
        else
           config.addDefine("K_NODEBUG")
           config.addOption("silent")
           print "[ \033[92mOFF\033[0m ]\n"
        end
    
    unix = config.addScope("unix")
    unix.addVariable("MOC_DIR", ".moc")
    unix.addVariable("UI_DIR", ".ui")
    unix.addVariable("OBJECTS_DIR", ".obj")

     # The file ktconfig.pri contains all the global variables for the compilation process        
    config.save("ktconfig.pri")
    conf.createMakefiles
    
rescue => err
    Info.error << "Configure failed. error was: #{err.message}\n"
    if $DEBUG
        puts err.backtrace
    end
end

