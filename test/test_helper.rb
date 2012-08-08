$LOAD_PATH << File.expand_path( File.dirname(__FILE__) + '/../ext' )
$LOAD_PATH << File.expand_path( File.dirname(__FILE__) + '/..' )

require 'test/unit'

system("cd #{File.expand_path(File.join(File.dirname(__FILE__), '../ext/'))}; make clean")
system("cd #{File.expand_path(File.join(File.dirname(__FILE__), '../ext/'))}; ruby extconf.rb")
system("cd #{File.expand_path(File.join(File.dirname(__FILE__), '../ext/'))}; make")

require "ImageSimilarity"

def assert_array_similarity(expected, actual, message=nil)
  full_message = build_message(message, "<?> expected but was\n<?>.\n", expected, actual)
  assert_block(full_message) { (expected.size ==  actual.size) && (expected - actual == []) }
end
