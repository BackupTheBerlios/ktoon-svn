#!/usr/bin/ruby

require 'qonf/configure'
require 'qonf/info'

require 'rbconfig'

begin
    conf = RQonf::Configure.new(ARGV)

    if conf.hasArgument?("help") or conf.hasArgument?("h")
       puts <<_EOH_
Use: #{$0} [options]
     options:
     --help, -h:					Show this message
     --prefix=[prefix], -prefix [prefix]:		Sets prefix
     --with-debug:					Enable debug
_EOH_
     exit 0
end

File.open("qonf/komconfig.rb", "w") { |file|
    file << %@
module RQonf
    CONFIG = {}
    CONFIG["prefix"] = "#{conf.destdir}"
    CONFIG["libdir"] = "#{conf.destdir}/lib"
    CONFIG["includepath"] = "#{conf.destdir}/include"
end
@
}
    conf.setTestDir("configure.tests")
    conf.verifyQtVersion("4.5.0")
    conf.createTests

    config = RQonf::Config.new
    conf.runTests(config)

    config.addModule("core")
    config.addModule("gui")
    config.addModule("svg")
    config.addModule("xml")
    config.addModule("network")

    Info.info << "Debug support... "

    if conf.hasArgument?("with-debug")
       config.addDefine("K_DEBUG")
       print "[ ON ]\n"
    else
       config.addDefine("K_NODEBUG")
       config.addOption("silent")
       print "[ OFF ]\n"
    end

    config.addDefine(%@KLIB_PREFIX='\\\\"'#{conf.destdir}'\\\\"'@)

    unix = config.addScope("unix")
    unix.addVariable("MOC_DIR", ".moc")
    unix.addVariable("UI_DIR", ".ui")
    unix.addVariable("OBJECTS_DIR", ".obj")

    config.save("kom/komconfig.pri")
    destdir_valid = conf.destdir

    while not File.exists?(destdir_valid)
          destdir_valid = File.expand_path(destdir_valid+"/..")
    end

    qonfdestdir = ( (File.stat(Config::CONFIG['sitelibdir']).writable? or not File.stat(destdir_valid).writable? ) ? Config::CONFIG['sitelibdir'] : conf.destdir )
    File.open("qonf.pri", "w") { |file|
        file << %@
        QMAKE_STRIP = echo
        INSTALLS += qonf
        qonf.files += qonf/*.rb
        qonf.path += #{qonfdestdir}/qonf\n@
    }

    conf.createMakefiles

    conf.qmake.run
    RQonf::Makefile::override("#{Dir.getwd}/Makefile", "", conf.statusFile)

    puts %@ Note: Make sure "#{qonfdestdir}" is in your RUBYLIB@
	
rescue => err
    Info.error << "Configure failed. Error was: #{err.message}\n"
    if $DEBUG
       puts err.backtrace
    end
end

