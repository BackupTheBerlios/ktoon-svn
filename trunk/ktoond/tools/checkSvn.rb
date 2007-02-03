#!/usr/bin/ruby

modified = []
added = []
deleted = []
conflicted = []
combined = []
unknown = []

IO.popen("svn st") { |svn|
	lines = svn.readlines
	
	lines.each { |line|
		case line
			when /^M\s+([\w\W]*)$/
				modified << $1
			when /^A\s+\+{0,1}\s+([\w\W]*)$/
				added << $1
			when /^D\s+([\w\W]*)$/
				deleted << $1
			when /^C\s+([\w\W]*)$/
				conflicted << $1
			when /^G\s+([\w\W]*)$/
				combined << $1
			when /^\?\s+([\w\W]*)$/
				opt = $1
				if not opt =~ /moc_|Makefile|\.ui|\.moc|qmake_|\.a|\.so|\.pcs|\.obj|qrc_/
					unknown << opt
				end
		end
	}
}

def show(list, title)
	if not list.empty?
		puts title
		list.sort!
		list.each { |line|
			puts "\t#{line}"
		}
	end
end



show(modified, "Modified")
show(added, "Added")
show(deleted, "Deleted")
show(conflicted, "Conflicts")
show(combined, "Combined")
show(unknown, "Unknown")
