#require './ImageSimilarity'

module ImageSimilarity

  attach_function :do_phash, [:string, :pointer], :int, :blocking => true
  attach_function :phash_hamming_distance, [:uint64, :uint64], :int, :blocking => true

  class << self
    # Get image file hash using <tt>ph_dct_imagehash</tt>
    def phashimage_signature(path)

      hash_p = FFI::MemoryPointer.new :ulong_long
      if -1 != do_phash(path.to_s, hash_p)
        hash = hash_p.get_uint64(0)
        hash_p.free

        PHash.new(hash)
      end
    end

    # Get distance between two image hashes using <tt>ph_hamming_distance</tt>
    def phash_distance(hash_a, hash_b)
       hash_a.is_a?(PHash) or raise ArgumentError.new('hash_a is not an PHash')
       hash_b.is_a?(PHash) or raise ArgumentError.new('hash_b is not an PHash')

       #bits_a =  hash_a.data.to_s.unpack('b*')[0].split(//)
       #bits_b = hash_b.data.to_s.unpack('b*')[0].split(//)
       #puts "Distance b/w #{hash_a.data.to_s.unpack('b*')[0].split(//)} & #{hash_b.data.to_s.unpack('b*')}"

       return phash_hamming_distance(hash_a.data, hash_b.data)
    end

    # Get similarity from hamming_distance
    def phash_similarity(hash_a, hash_b)
      1 - phash_distance(hash_a, hash_b) / 64.0
    end

  end

  # Class to store image hash and compare to other
  class PHash < Signature
  end

  # Class to store image file hash and compare to other
  class PHashImage < FileSignature
  end

end
