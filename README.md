# ExoPlayer ISO23008-1 MMT extension #

The exoplayer extension provides MMT playback support (ISO23008-1 and extended support for A/331:2020).

## License note ##

Please note that whilst the code in this repository is licensed under
[MIT], using this extension also requires building and including one or
more external libraries as described below. These are licensed separately.

[MIT]: libatsc3: https://github.com/jjustman/libatsc3

## Build instructions ##

To use this extension you need to clone the ExoPlayer repository and depend on
its modules locally. Instructions for doing this can be found in ExoPlayer's
[top level README][].

In addition, it's necessary to build the extension's native components as
follows:

## Using the extension ##

Once you've followed the instructions above to check out, build and depend on
the extension...


Example:

Grab the following MMT extracted PCAP file from:

    https://github.com/jjustman/atsc3_stltp_pcaps/blob/master/2020-12-10-mmt-testing-flows-stltp-and-demux/2020-12-10-MMT-only-hv-lab-239.255.50.9.5009.pcap

Sample Activity (kotlin):

        val path = "/storage/emulated/0/Download/2020-12-10-MMT-only-hv-lab-239.255.50.9.5009.pcap"

        val videoUri = Uri.parse(path)

        view?.findViewById<View>(R.id.progress_bar)!!.visibility = View.GONE

        val trackSelector = DefaultTrackSelector(AdaptiveTrackSelection.Factory())
        val player = ExoPlayerFactory.newSimpleInstance(requireContext(), MMTRenderersFactory(context), trackSelector, MMTLoadControl())

        val playerView = view?.findViewById<PlayerView>(R.id.receiver_media_player)!!
        playerView.player = player
        playerView.requestFocus()

        val mediaSource = MMTMediaSource.Factory({
            PcapUdpDataSource()
        }, {
            arrayOf(MMTExtractor())
        }).apply {
            //setLoadErrorHandlingPolicy(createDefaultLoadErrorHandlingPolicy())
        }.createMediaSource(videoUri)

        player.prepare(mediaSource)
        player.playWhenReady = true


## Links ##

## Release Notes ##
2020-12-22 - version 0.1 - jjustman@nbgp.org
