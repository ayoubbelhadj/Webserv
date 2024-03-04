<?php
$targetDirectory = 'uploads/';

if (!file_exists($targetDirectory) && !mkdir($targetDirectory, 0777, true)) {
    die('Failed to create folder');
}

if (!empty($_FILES['files']['name'])) {
    foreach ($_FILES['files']['name'] as $key => $name) {
        $targetFile = $targetDirectory . basename($name);
        move_uploaded_file($_FILES['files']['tmp_name'][$key], $targetFile);
    }

    echo json_encode(['message' => 'Folder uploaded successfully']);
} else {
    echo json_encode(['message' => 'No files to upload']);
}
?>