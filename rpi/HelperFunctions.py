class HelperFunctions:
    def __init__(self, options=None)
        self.options = options

def print_verbose(text, indent=0)
    if HelperFunctions().get_options()['verbose']:
        print('\t'*indent, end='')
        print(text)
