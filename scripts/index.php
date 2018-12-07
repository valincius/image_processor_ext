<?php
    $img = image_load("flower.bmp");
    image_info($img);

    // image_apply_mask($img, [
    //     1,4,6,4,1,
    //     4,16,24,16,4,
    //     6,24,-476,24,5,
    //     4,16,24,16,4,
    //     1,4,6,4,1,
    // ], -256);
    // image_flip_x($img);
    // image_apply_color($img, 255, 255, 40);

    $img_base64 = image_base64($img);
?>

<img src="data:image/bmp;base64,<?=$img_base64?>" />