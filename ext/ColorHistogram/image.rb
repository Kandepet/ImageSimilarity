#require './ImageSimilarity'
require 'rubygems'
require 'RMagick'

module ImageSimilarity

   class << self
      def colorhistogramimage_signature(path)

         img = Magick::Image.read(path).first

         ColorHistogram.new(img)
      end

      # Get similarity from hamming_distance
      def colorhistogram_similarity(histogram_a, histogram_b)
         histogram_a.is_a?(ColorHistogram) or raise ArgumentError.new('histogram_a is not a ColorHistogram')
         histogram_b.is_a?(ColorHistogram) or raise ArgumentError.new('histogram_b is not a ColorHistogram')

         histogram_a.intersection(histogram_b)
      end
   end

   # Class to store image histogram and compare to other
   class ColorHistogram < Signature
      attr_reader :hist

      def initialize(image)
         @hist = [0] * 64
         if image.class == Magick::Image
            image.each_pixel do |pixel, c, r|
               @hist[to_bin(a(pixel.red), a(pixel.green), a(pixel.blue))] += 1
            end
         else
            @hist = image
         end
      end

      def self.parse(str)
         self.new(str.split(":").map{|l| l.to_i})
      end

      def serialize
         @hist.map{|c| c.to_s}.join(":")
      end
      alias :to_s :serialize

      def inspect
         "#<ColorHistogram: #{serialize}>"
      end

      def intersection(other)
         d = @hist.inject(0){|i,j| i + j}
         intsec = @hist.zip(other.hist).map{|a| a.min}.inject(0){|i,j| i + j}
         intsec.to_f / d
      end

      private

      def a(n)
         #puts "a(#{n})"
         (n/16384).floor
      end

      def to_bin(r, g, b)
         val = (r * 4 * 4 + g * 4 + b * 1)
         #puts "Returning #{r}, #{g}, #{b} => #{val}"
         return val
      end
   end

   # Class to store image file histogram and compare to other
   class ColorHistogramImage < FileSignature
   end

end
