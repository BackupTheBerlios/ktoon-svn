
module Kernel
	def close_output
		close_stderr
# 		close_stdout
	end
	
	def open_output
		open_stderr
# 		open_stdout
	end
	
	def close_stderr
		@stderr = $stderr.clone
		$stderr.close
	end
	
	def open_stderr
		if @stderr
			$stderr = @stderr
		end
	end
	
	def close_stdout
		@stdout = $stdout.clone
		$stdout.close
	end
	
	def open_stdout
		if @stdout
			$stdout = @stdout
		end
	end
end

