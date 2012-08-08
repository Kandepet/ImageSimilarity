# -*- encoding: utf-8 -*-
#
# SimilarImage
#

require 'rubygems'
require 'RMagick'


module SimilarImage

  def self.color_histogram(file)
    img = Magick::Image.read(file).first
    puts img
    ColorHistogram.new(img)
  end

  ## classes:

  class ColorHistogram
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
  end   # of ColorHistogram


end   # of SimilarImage

#puts "Calculating histogram for: orig"
#puts File.exists?("./ExposureCompensated/IMG_3965.JPG")
#orig = SimilarImage.color_histogram "./ExposureCompensated/IMG_3965.JPG"

#puts "Calculating histogram for: dup"
#dup = SimilarImage.color_histogram "./ExposureCompensated/IMG_DUP_3965.JPG"
#puts "Calculating histogram for: small"
#small = SimilarImage.color_histogram "./ExposureCompensated/IMG_small_3965.JPG"
#puts "Calculating histogram for: expoed"
#expoed = SimilarImage.color_histogram "./ExposureCompensated/IMG_3966.JPG"
#puts "Calculating histogram for: cropped"
#cropped = SimilarImage.color_histogram "./ExposureCompensated/IMG_3965_CROP.JPG"
#
#puts "Computing similarity"
#puts "orig % dup #{orig.intersection(dup)}"
#puts "orig % small #{orig.intersection(small)}"
#puts "orig % expoed #{orig.intersection(expoed)}"
#puts "orig % cropped #{orig.intersection(cropped)}"

