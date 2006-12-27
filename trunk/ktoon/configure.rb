#!/usr/bin/ruby

$: << "qonf"

require 'qonf/configure'
require 'qonf/info'

begin
	conf = RQonf::Configure.new(ARGV)
	conf.setTestDir("configure.tests")
	conf.verifyQtVersion("4.2.0")
	conf.createTests
	config = conf.createConfig("ktconfig.pri")
	
	config.addModule("core")
	config.addModule("gui")
	config.addModule("svg")
	config.addModule("xml")
	
	config.addDefine("VERSION=0.9+svn")
	config.addDefine('VERSION_STR=\\\\\"0.9+svn\\\\\"')
	
	unix = config.addScope("unix")
	unix.addVariable("MOC_DIR", ".moc")
	unix.addVariable("UI_DIR", ".ui")
	unix.addVariable("OBJECTS_DIR", ".obj")
	
	

	config.save
	
	conf.createMakefiles
	
rescue => err
	Info.error << "Configure failed. error was: #{err.message}\n"
	if $DEBUG
		puts err.backtrace
	end
end

