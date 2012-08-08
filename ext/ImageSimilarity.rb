require 'ffi'

module ImageSimilarity
  class Data
    attr_reader :data, :length
    def initialize(data, length = nil)
      @data, @length = data, length
    end
  end

  class Signature < Data
    def similarity(other, *args)
      ImageSimilarity.send("#{self.class.signature_type}_similarity", self, other, *args)
    end

    def self.signature_type
      #@hash_type ||= self.name.split('::').last.sub(/Hash$/, '').downcase
      @signature_type ||= self.name.split('::').last.downcase
    end
  end

  class FileSignature
    attr_reader :path

    # File path
    def initialize(path)
      @path = path
    end

    # Init multiple image instances
    def self.for_paths(paths, *args)
      paths.map do |path|
        new(path, *args)
      end
    end

    # Cached signature
    def signature
      @signature ||= compute_signature
    end

    def compute_signature
      ImageSimilarity.send("#{self.class.signature_type}_signature", @path)
    end

    # Similarity with other signature
    def similarity(other, *args)
      signature.similarity(other.signature, *args)
    end
    alias_method :%, :similarity

    def self.signature_type
      @signature_type ||= self.name.split('::').last.downcase
    end
  end

  extend FFI::Library

  # Get path relative to this file
  path = File.dirname(__FILE__)

  # Load Average Hash
  ffi_lib "#{path}/PHash/PHash.so", "#{path}/AverageHash/AverageHash.so"

  autoload :AverageHashImage, "#{path}/AverageHash/image"
  autoload :PHashImage, "#{path}/PHash/image"

  autoload :ColorHistogramImage, "#{path}/ColorHistogram/image"
  autoload :HaarWaveletImage, "#{path}/HaarWavelet/image"
end
