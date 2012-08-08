require File.expand_path( File.dirname(__FILE__) + '/test_helper.rb' )


class CompareImageTest < Test::Unit::TestCase
   def setup
      @img_path = File.dirname(__FILE__)
   end

  def test_average_hash_image_similarity
    original = ImageSimilarity::AverageHashImage.new("#{@img_path}/TestImages/Orig.jpg")
    duplicate = ImageSimilarity::AverageHashImage.new("#{@img_path}/TestImages/Dup.jpg")
    rotated = ImageSimilarity::AverageHashImage.new("#{@img_path}/TestImages/Rotated.jpg")
    resized = ImageSimilarity::AverageHashImage.new("#{@img_path}/TestImages/Resized.jpg")
    exposure_compensated = ImageSimilarity::AverageHashImage.new("#{@img_path}/TestImages/EC.jpg")
    cropped = ImageSimilarity::AverageHashImage.new("#{@img_path}/TestImages/Crop.jpg")

    # Original & Duplicate image
    assert_equal 1.0, original.similarity(duplicate)

    # Original & Rotated by 90
    assert_in_delta 0.53125, original.similarity(rotated), 0.01

    #Original & Resized to 10% of original
    assert_in_delta 1.0, original.similarity(resized), 0.01

    #Original & exposure compensated -2/3
    assert_in_delta 0.953125, original.similarity(exposure_compensated), 0.01

    #Original & cropped to just upper left quadrant of original image
    assert_in_delta 0.625, original.similarity(cropped), 0.01
  end

  def test_phash_image_similarity
    original = ImageSimilarity::PHashImage.new("#{@img_path}/TestImages/Orig.jpg")
    duplicate = ImageSimilarity::PHashImage.new("#{@img_path}/TestImages/Dup.jpg")
    rotated = ImageSimilarity::PHashImage.new("#{@img_path}/TestImages/Rotated.jpg")
    resized = ImageSimilarity::PHashImage.new("#{@img_path}/TestImages/Resized.jpg")
    exposure_compensated = ImageSimilarity::PHashImage.new("#{@img_path}/TestImages/EC.jpg")
    cropped = ImageSimilarity::PHashImage.new("#{@img_path}/TestImages/Crop.jpg")

    # Original & Duplicate image
    assert_equal 1.0, original.similarity(duplicate)

    # Original & Rotated by 90
    assert_in_delta 0.5, original.similarity(rotated), 0.01

    #Original & Resized to 10% of original
    assert_in_delta 0.90625, original.similarity(resized), 0.01

    #Original & exposure compensated -2/3
    assert_in_delta 0.875, original.similarity(exposure_compensated), 0.01

    #Original & cropped to just upper left quadrant of original image
    assert_in_delta 0.59375, original.similarity(cropped), 0.01
  end


  def test_color_histogram_image_similarity
    original = ImageSimilarity::ColorHistogramImage.new("#{@img_path}/TestImages/Orig.jpg")
    duplicate = ImageSimilarity::ColorHistogramImage.new("#{@img_path}/TestImages/Dup.jpg")
    rotated = ImageSimilarity::ColorHistogramImage.new("#{@img_path}/TestImages/Rotated.jpg")
    resized = ImageSimilarity::ColorHistogramImage.new("#{@img_path}/TestImages/Resized.jpg")
    exposure_compensated = ImageSimilarity::ColorHistogramImage.new("#{@img_path}/TestImages/EC.jpg")
    cropped = ImageSimilarity::ColorHistogramImage.new("#{@img_path}/TestImages/Crop.jpg")

    # Original & Duplicate image
    assert_equal 1.0, original.similarity(duplicate)

    # Original & Rotated by 90
    assert_in_delta 0.99, original.similarity(rotated), 0.01

    #Original & Resized to 10% of original
    assert_in_delta 0.01, original.similarity(resized), 0.01

    #Original & exposure compensated -2/3
    assert_in_delta 0.81, original.similarity(exposure_compensated), 0.01

    #Original & cropped to just upper left quadrant of original image
    assert_in_delta 0.02, original.similarity(cropped), 0.01
  end

  def test_haar_wavelet_transform_image_similarity
    original = ImageSimilarity::HaarWaveletImage.new("#{@img_path}/TestImages/Orig.jpg")
    duplicate = ImageSimilarity::HaarWaveletImage.new("#{@img_path}/TestImages/Dup.jpg")
    rotated = ImageSimilarity::HaarWaveletImage.new("#{@img_path}/TestImages/Rotated.jpg")
    resized = ImageSimilarity::HaarWaveletImage.new("#{@img_path}/TestImages/Resized.jpg")
    exposure_compensated = ImageSimilarity::HaarWaveletImage.new("#{@img_path}/TestImages/EC.jpg")
    cropped = ImageSimilarity::HaarWaveletImage.new("#{@img_path}/TestImages/Crop.jpg")

    # Original & Duplicate image
    assert_equal 1.0, original.similarity(duplicate)

    # Original & Rotated by 90
    assert_in_delta 0.16, original.similarity(rotated), 0.01

    #Original & Resized to 10% of original
    assert_in_delta 0.94, original.similarity(resized), 0.01

    #Original & exposure compensated -2/3
    assert_in_delta 0.88, original.similarity(exposure_compensated), 0.01

    #Original & cropped to just upper left quadrant of original image
    assert_in_delta 0.30, original.similarity(cropped), 0.01
  end


end
