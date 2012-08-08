#require './ImageSimilarity'

module ImageSimilarity

  attach_function :do_averagehash, [:string, :pointer], :int, :blocking => true
  attach_function :averagehash_hamming_distance, [:uint64, :uint64], :int, :blocking => true

  class << self
    def averagehashimage_signature(path)

      hash_p = FFI::MemoryPointer.new :ulong_long
      if -1 != do_averagehash(path.to_s, hash_p)
        hash = hash_p.get_uint64(0)
        hash_p.free

        AverageHash.new(hash)
      end
    end

    def averagehash_distance(hash_a, hash_b)
       hash_a.is_a?(AverageHash) or raise ArgumentError.new('hash_a is not an AverageHash')
       hash_b.is_a?(AverageHash) or raise ArgumentError.new('hash_b is not an AverageHash')

       return averagehash_hamming_distance(hash_a.data, hash_b.data)
    end


    # Get similarity from hamming_distance
    def averagehash_similarity(hash_a, hash_b)
      1 - averagehash_distance(hash_a, hash_b) / 64.0
    end

  end

  # Class to store image hash and compare to other
  class AverageHash < Signature
  end

  # Class to store image file hash and compare to other
  class AverageHashImage < FileSignature
  end

end
