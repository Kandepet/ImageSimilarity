#require './ImageSimilarity'

require "#{File.dirname(__FILE__)}/imgseek"

module ImageSimilarity

   class << self
      def haarwaveletimage_signature(path)
         img = Similarity::ImgSeek.new.load(path)
         HaarWavelet.new(img)
      end

      # Get similarity from hamming_distance
      def haarwavelet_similarity(signature_a, signature_b)
         signature_a.is_a?(HaarWavelet) or raise ArgumentError.new('signature_a is not a HaarWavelet')
         signature_b.is_a?(HaarWavelet) or raise ArgumentError.new('signature_b is not a HaarWavelet')

         signature_a.intersection(signature_b)
      end
   end

   # Class to store image signature and compare to other
   class HaarWavelet < Signature
      attr_reader :signature

      def initialize(signature)
         @signature = signature
      end

      def intersection(other)
         @signature.similarity(other.signature)
      end
   end

   # Class to store image file signature and compare to other
   class HaarWaveletImage < FileSignature
   end

end
