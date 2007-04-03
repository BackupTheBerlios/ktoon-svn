#!/usr/bin/ruby

require 'qonf/configure'
require 'qonf/info'

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
	
	conf.verifyQtVersion("4.2.0")
	
	config = RQonf::Config.new
	
	config.addModule("core")
	config.addModule("gui")
	config.addModule("svg")
	config.addModule("xml")
	config.addModule("network")

	config.addLib("-ldgui")
	config.addLib("-ldcore")
	config.addLib("-ldsound")
	config.addLib("-L#{ENV["KTOON_HOME"]}/lib -lktoon -lstore -lktbase")

	config.addIncludePath(ENV["KTOON_HOME"]+"/include/ktoon")
	config.addIncludePath(ENV["KTOON_HOME"]+"/include/store")
	config.addIncludePath(ENV["KTOON_HOME"]+"/include/base")
	
	
	if not conf.hasArgument?("with-debug")
		config.addDefine("D_NODEBUG")
		config.addOption("silent")
	end
	
	unix = config.addScope("unix")
	unix.addVariable("MOC_DIR", ".moc")
	unix.addVariable("UI_DIR", ".ui")
	unix.addVariable("OBJECTS_DIR", ".obj")
	
	config.save("ktadmin_config.pri")
	conf.createMakefiles
	
rescue => err
	Info.error << "Configure failed. error was: #{err.message}\n"
	if $DEBUG
		puts err.backtrace
	end
end

