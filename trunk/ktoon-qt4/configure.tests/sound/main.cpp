#include <QSound>

int main()
{
	
	if ( ! QSound::isAvailable() )
	{
#error "No sound"
	}

	return 0;
}


