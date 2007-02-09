
module KToonD

class Runner
	def initialize
		@path = "ktoond"
	end
	
	def run
		exec(@path) if fork == nil
	end
	
	def stop
		`pkill #{@path}`
	end
	
	def restart
		stop
		run
	end
end


end #module
